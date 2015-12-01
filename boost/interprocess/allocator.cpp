#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <cassert>

using namespace boost::interprocess;

int main ()
{
   //Remove shared memory on construction and destruction
   struct shm_remove
   {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   } remover;

   //Create shared memory
   managed_shared_memory segment(create_only,
                                 "MySharedMemory",  //segment name
                                 65536);

   //Create an allocator that allocates ints from the managed segment
   allocator<int, managed_shared_memory::segment_manager>
      allocator_instance(segment.get_segment_manager());

   //Copy constructed allocator is equal
   allocator<int, managed_shared_memory::segment_manager>
      allocator_instance2(allocator_instance);
   assert(allocator_instance2 == allocator_instance);

   //Allocate and deallocate memory for 100 ints
   allocator_instance2.deallocate(allocator_instance.allocate(100), 100);

   return 0;
}