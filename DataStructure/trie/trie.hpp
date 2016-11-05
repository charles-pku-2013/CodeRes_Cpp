#ifndef _TRIE_HPP_
#define _TRIE_HPP_

#include <memory>
#include <set>
#include <vector>
#include <deque>
#include <iostream>
#include <cassert>


template< typename T, typename ElemCmp = std::less<T> >
class Trie {
public:
    typedef T                                  elem_type;
    typedef typename std::shared_ptr<T>        elem_pointer;

    struct ElemPtrCmp {
        constexpr bool operator() (const elem_pointer &lhs, const elem_pointer &rhs) const
        { return ElemCmp()(*lhs, *rhs); }
    };

    typedef std::set<elem_pointer, ElemPtrCmp> ElemSet;

    struct Node : std::enable_shared_from_this<Node> {
        typedef typename std::shared_ptr<Node>     pointer;
        typedef typename std::weak_ptr<Node>       weak_pointer;

        struct PointerCmp {
            constexpr bool operator() (const pointer &lhs, const pointer &rhs) const
            { return ElemCmp()(lhs->data(), rhs->data()); }
        };

        typedef typename std::set<pointer, PointerCmp> ChildrenSet;

        Node() : m_fWeight(0.0) {}
        Node( const elem_pointer &_Data ) : m_pData(_Data), m_fWeight(0.0) {}

        T& data() { assert(m_pData); return *m_pData; }
        const T& data() const { assert(m_pData); return *m_pData; }
        elem_pointer dataPtr() { return m_pData; }

        void setParent(const pointer &_Parent)
        { m_wpParent = _Parent; }
        pointer parent() const
        { return m_wpParent.lock(); }

        std::pair<pointer, bool>
        addChild(const pointer &_Child)
        {
            assert(_Child->m_pData);
            _Child->setParent( this->shared_from_this() );
            auto ret = m_setChildren.insert(_Child);
            return std::make_pair(*(ret.first), ret.second);
        }

        pointer child( const pointer &p )
        {
            auto it = m_setChildren.find(p);
            return (it == m_setChildren.end() ? pointer() : *it);
        }

        pointer child( const elem_pointer &p )
        {
            Node node(p);
            pointer pNode(&node, [](Node*){/* Do nothing */});
            return child(pNode);
        }

        pointer child( const elem_type& data )
        {
            elem_pointer pElem(
                const_cast<elem_type*>(&data),
                [](elem_type*){/* Do nothing */}
            );
            return child(pElem);
        }

        ChildrenSet& children() { return m_setChildren; }

        bool removeChild( const pointer &pChild )
        { return m_setChildren.erase( pChild ); }

        bool removeSelf()
        {
            pointer pParent = parent();
            if (pParent)
                pParent->removeChild( this->shared_from_this() ); // NOTE!!! this must,  error: there are no arguments to ‘shared_from_this’ that depend on a template parameter, so a declaration of ‘shared_from_this’ must be available
        }

        void setWeight(const double &w) { m_fWeight = w; }
        double weight() const { return m_fWeight; }

        void getPath( std::deque<elem_pointer> &path )
        {
            path.clear();
            pointer pNode = this->shared_from_this();
            for (; pNode->parent(); pNode = pNode->parent())
                path.push_front( pNode->dataPtr() );
        }

        void traverse( std::ostream &os, std::size_t level )
        {
            for (size_t i = 0; i < level; ++i)
                os << "\t";

            if (m_pData)
                os << *m_pData;
            else
                os << "#ROOT#";
            os << std::endl;

            for (auto &np : children())
                np->traverse(os, level+1);
        }

        std::size_t countNodes()
        {
            std::size_t count = 1;
            for (auto &p : children())
                count += p->countNodes();
            return count;
        }

        elem_pointer    m_pData;
        double          m_fWeight;
        ChildrenSet     m_setChildren;
        weak_pointer    m_wpParent;
    }; // struct Node

public:
    Trie()
    { m_pRoot = std::make_shared<Node>(); }

    virtual ~Trie() = default;

    typename Node::pointer root()
    { return m_pRoot; }

    ElemSet& elemSet()
    { return m_setElems; }

    template<typename Iter>
    typename Node::pointer addPath( Iter beg, Iter end )
    {
        typename Node::pointer curNode = m_pRoot;

        for (; beg != end; ++beg) {
            auto ret = m_setElems.insert( std::make_shared<T>(*beg) );
            auto pData = *(ret.first);
            auto newNode = std::make_shared<Node>(pData);
            auto acRet = curNode->addChild(newNode);
            curNode = acRet.first;
        } // for

        return curNode;
    }

    typename Node::pointer
    addPath( const std::vector<T> &vec )
    { return addPath(vec.begin(), vec.end()); }

    std::pair<typename Node::pointer, bool>
    addNode(const typename Node::pointer &parent, 
            const typename Node::pointer &newNode)
    {
        assert(parent);
        return parent->addChild(newNode);
    }

    std::pair<typename Node::pointer, bool>
    addNode(const typename Node::pointer &parent, 
            const T& data)
    {
        auto ret = m_setElems.insert( std::make_shared<T>(data) );
        auto pData = *(ret.first);
        auto newNode = std::make_shared<Node>(pData);
        return addNode(parent, newNode);
    }

    template<typename Iter>
    std::pair<typename Node::pointer, bool>
    lookup( Iter beg, Iter end )
    {
        typedef typename Iter::value_type DataType;
        typename Node::pointer curNode = m_pRoot;
        
        for (; beg != end; ++beg) {
            std::shared_ptr<DataType> pData(
                const_cast<DataType*>(&(*beg)),
                [](DataType*){}
            );

            Node _node(pData);
            typename Node::pointer pNode(
                &_node,
                [](Node*){}
            );

            auto it = curNode->children().find( pNode );
            if (it == curNode->children().end())
                return std::make_pair(curNode, false);
            
            curNode = *it;
        } // for

        return std::make_pair(curNode, true);
    }

    std::ostream& traverse( std::ostream &os )
    {
        root()->traverse(os, 0);
        return os;
    }

    std::size_t countNodes()
    { return root()->countNodes()-1; }

    void syncElems()
    {
        auto it = m_setElems.begin();
        for (; it != m_setElems.end();) {
            if (it->use_count() == 1)
                m_setElems.erase(it++);
            else
                ++it;
        } // for
    }

protected:
    typename Node::pointer    m_pRoot;

    // NOTE!!! 以下方法为了减少重复元素占用内存，但只适合于元素插入后就不改变的情况
    // 若元素中还有其他filed需要插入后改变则不可以使用
    ElemSet                   m_setElems; // check when remove node
};


#endif

