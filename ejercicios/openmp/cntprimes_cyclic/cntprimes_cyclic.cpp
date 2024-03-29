#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

bool is_prime(size_t number)
{
    if ( number < 2 ) return false;
    if ( number == 2 ) return true;
    if ( number % 2 == 0 ) return false;

    for ( size_t i = 3, last = (size_t)(double)sqrt(number); i <= last; i += 2 )
        if ( number % i == 0 )
            return false;

    return true;
}

int main(int argc, char* argv[])
{
    size_t limit = 0;
    
    int thread_count = omp_get_max_threads();
    if(argc >= 3)
        thread_count = atoi(argv[2]);

    if ( argc >= 2 )
        limit = strtoull(argv[1], NULL, 10);
    else
        return (void)fprintf(stderr, "usage: cntprimes <LIMIT> [WORKERS]\n"), 1;

    size_t prime_count = 0;

    #pragma omp parallel for num_threads(thread_count) schedule(runtime)\
        default(none) shared(limit) reduction(+:prime_count)
    for ( size_t current = 2; current <= limit; ++current )
        if ( is_prime(current) )
            ++prime_count;
    fprintf(stdout, "%zu primes found between 2 and %zu\n", prime_count, limit);
    return 0;
}