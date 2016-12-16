/*
 * class A {
 * protected:
 *     int a;
 * public:
 *     A() {a = 10;}
 *     virtual void update() {a++;}
 *     int access() {update(); return a;}
 * };
 */

struct A;

typedef struct {
    void (*A)(struct A*);
    void (*update)(struct A*);
    int (*access)(struct A*);
} A_functable;

typedef struct A{
    int a;
    A_functable *vmt;
} A;

void A_A(A *this);
void A_update(A* this);
int A_access(A* this);

A_functable A_vmt = {A_A, A_update, A_access};

void A_A(A *this) {this->vmt = &A_vmt; this->a = 10;}
void A_update(A* this) {this->a++;}
int A_access(A* this) {this->vmt->update(this); return this->a;}

/*
class B: public A {
public:
    void update() {a--;}
};
*/

struct B;

typedef struct {
    void (*B)(struct B*);
    void (*update)(struct B*);
    int (*access)(struct A*);
} B_functable;

typedef struct B {
    A inherited;
} B;

void B_B(B *this);
void B_update(B* this);

B_functable B_vmt = {B_B, B_update, A_access};

void B_B(B *this) {A_A(this); this->inherited.vmt = &B_vmt; }
void B_update(B* this) {this->inherited.a--;}
int B_access(B* this) {this->inherited.vmt->update(this); return this->inherited.a;}

int main() {
    A x;
    B y;
    A_A(&x);
    B_B(&y);
    printf("%d\n", x.vmt->access(&x));
    printf("%d\n", y.inherited.vmt->access(&y));
}
