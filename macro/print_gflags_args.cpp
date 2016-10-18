/*
 * NOTE!!! #name 相当于加上了引号
 */

static
void print_args()
{
#define PRINT_ARG(name) \
    do { \
        std::cerr << #name << " = " << FLAGS_##name << std::endl; \
    } while (0)

    PRINT_ARG(alpha);
    PRINT_ARG(beta);
    PRINT_ARG(mh);
    PRINT_ARG(prefix);
    PRINT_ARG(niter);
    PRINT_ARG(k);
    PRINT_ARG(ntop);
    PRINT_ARG(bin);
    PRINT_ARG(model);
    PRINT_ARG(info);
    PRINT_ARG(vocab);
    PRINT_ARG(topics);
    PRINT_ARG(z);
    PRINT_ARG(estimate);
    PRINT_ARG(inference);
    PRINT_ARG(writeinfo);
    PRINT_ARG(dumpmodel);
    PRINT_ARG(dumpz);
    PRINT_ARG(perplexity);

#undef PRINT_ARG
}
