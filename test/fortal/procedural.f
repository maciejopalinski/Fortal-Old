package fortal.examples.procedural;

import fortal.system.System;

int pow(int a, int b)
{
    if (b < 0) return pow(a, b + 1) / a;
    else if (b > 0) return pow(a, b - 1) * a;
    else return 1;
}

void main()
{
    int x = 5 + 1;

    pow(2, 8);
}

EntryPoint = fortal.examples.procedural#main