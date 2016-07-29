/*
 * http://www.tolon.co.uk/2012/08/boost-context/
 */
#include <boost\context\all.hpp>
 
namespace ctx = boost::ctx;
 
ctx::fcontext_t fc_avg, fc_input, fc_main;
bool b_quit = false;
 
// --- The Average Task ---
// Computes the average of the input data and 
// then yields execution back to the main task
//-------------------------
// struct average_args - data for the task
// average_yield() - wrapper for yielding execution back to the main task
// average_task() - the task logic
// ------------------------
struct average_args
{
    int * source;
    int sum;
    int count;
    int average;
    int task_;
};
void average_yield() { ctx::jump_fcontext(&fc_avg, &fc_main, 0); }
void average_task(intptr_t p)
{
    average_args* args = (average_args*)p;
    args->sum = 0;
    args->count = 0;
    args->average = 0;
    while (true)
    {
        args->sum += *args->source;
        ++args->count;
        args->average = args->sum / args->count;
        average_yield();
    }
 
    printf("ERROR: should not reach the end of average function\n");
}
 
// --- The Input Task ---
// Reads a number as input from the console and 
// then yields execution back to the main task
// ----------------------
// struct input_args - data for the task
// input_yield() - wrapper for yielding execution back to the main task
// input_task() - the task logic
// ----------------------
struct input_args
{
    average_args* aa;
    int * target;
    int task_;
};
void input_yield() { ctx::jump_fcontext(&fc_input, &fc_main, 0); }
void input_task(intptr_t p)
{
    input_args* pia = (input_args*)p;
 
    while (true)
    {
        printf("number: ");
        if (!scanf_s("%d", pia->target))
        {
            b_quit = true;
            return;
        }
     
        input_yield();
    }
 
    printf("ERROR: should not reach the end of input function\n");
}
 
void main()
{
    int share = -1;
    average_args aa = {&share};
    input_args ia = {&aa, &share};
    ctx::stack_allocator alloc;
 
    // construct the input task
    fc_input.fc_stack.base = alloc.allocate(ctx::minimum_stacksize());
    ctx::make_fcontext(&fc_input, input_task);
 
    // construct the average task
    fc_avg.fc_stack.base = alloc.allocate(ctx::minimum_stacksize());
    ctx::make_fcontext(&fc_avg, average_task);
 
    while (!b_quit)
    {
        ctx::jump_fcontext( &fc_main, &fc_input, (intptr_t) &ia);
        ctx::jump_fcontext( &fc_main, &fc_avg, (intptr_t) &aa);
        printf("sum=%d count=%d average=%d\n", aa.sum, aa.count, aa.average);
    }
     
    printf("main: done\n");
}
