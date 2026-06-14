#include <stdio.h>
#include <radio_instance.h>

int main(void)
{
    RadioInstance* const* instances = radio_instance_get_all();

    printf("swimtool: linked with libgbinder-radio (%s)\n",
        instances ? "instance list accessible" : "no instances");

    return 0;
}
