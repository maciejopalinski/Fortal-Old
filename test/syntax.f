import 'std';

int pow(int a, int b)
{
    if (b < 0) return pow(a, b + 1) / a;
    else if (b > 0) return pow(a, b - 1) * a;
    else return 1;
}

int nth_triangle_num = (int n) => {
    if (n == 1) return 1;
    else return nth_triangle_num(n - 1) + n;
}

int main()
{
    __builtin_puts('Hello, world!');

    __builtin_puts(pow(2, 8));
    __builtin_puts(pow(2, -8));
    __builtin_puts(nth_triangle_num(25));

    return 0;
}