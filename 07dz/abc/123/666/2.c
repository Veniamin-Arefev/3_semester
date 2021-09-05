#include <stdio.h>
#include <stdlib.h>

enum
{
    MULTIPLIER = 1103515245,
    INCREMENT = 12345,
    MODULE = 1u << 31u
};

struct RandomOperations;

typedef struct RandomGenerator {
    unsigned int currentValue;
    unsigned int multiplier;
    unsigned int increment;
    unsigned int module;
    const struct RandomOperations *ops;
} RandomGenerator;

int next(struct RandomGenerator *rg) {
    long long calculating = rg->currentValue;
    calculating *= rg->multiplier;
    calculating += rg->increment;
    calculating = calculating % rg->module;
    rg->currentValue = calculating;
    return rg->currentValue;
}

void destroy(struct RandomGenerator *rg) {
    free(rg);
}

struct RandomOperations {
    void ( *destroy )(struct RandomGenerator *);
    int ( *next )(struct RandomGenerator *);
} RandomOperations;

const struct RandomOperations randomOperations = {destroy, next};

RandomGenerator *random_create(int seed) {
    RandomGenerator *rg = malloc(sizeof(RandomGenerator));
    rg->ops = &randomOperations;
    rg->currentValue = seed;
    rg->multiplier = MULTIPLIER;
    rg->increment = INCREMENT;
    rg->module = MODULE;
    return rg;
}

int main(void) {
    RandomGenerator *rr = random_create(1234);
    for (int j = 0; j < 100; ++j) {
        printf("%d\n", rr->ops->next(rr));
    }
    rr->ops->destroy(rr);
}
