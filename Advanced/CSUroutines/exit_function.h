struct exit_function
{
/* flavor : enum 유형 -> Atomic 연산 : long int*/
    long int flavor;

    union {

        void (*at) (void);
        struct {
            void (*fn) (int status, void *arg);
            void *arg;
        } on;

        struct {
            void (*fn) (void *arg, int status);
            void *arg;
            void *dso_handle;
        } cxa;

    } func;
};