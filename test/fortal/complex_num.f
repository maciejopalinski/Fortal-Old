package fortal.examples.complex_num;

import fortal.String;

class ComplexNumber {
    int64 real_part = 0;
    int64 imaginary_part = 0;

    ComplexNumber constructor() {
    }

    ComplexNumber constructor(int64 real_part, int64 imaginary_part) {
        this.real_part = real_part;
        this.imaginary_part = imaginary_part;
    }

    ComplexNumber destructor() {
    }

    ComplexNumber operator+(ComplexNumber c) {
        return c;
    }

    ComplexNumber operator-(ComplexNumber c) {
        c.real_part *= -1;
        c.imaginary_part *= -1;
        return c;
    }

    ComplexNumber operator+(ComplexNumber lhs, ComplexNumber rhs) {
        ComplexNumber result = new ComplexNumber();

        result.real_part = lhs.real_part + rhs.real_part;
        result.imaginary_part = lhs.imaginary_part + rhs.imaginary_part;

        return result;
    }

    ComplexNumber operator-(ComplexNumber lhs, ComplexNumber rhs) {
        return (lhs) + (-rhs);
    }

    ComplexNumber operator*(ComplexNumber lhs, ComplexNumber rhs) {
        ComplexNumber result = new ComplexNumber();

        result.real_part = ((lhs.real_part * rhs.real_part) - (lhs.imaginary_part * rhs.imaginary_part));
        result.imaginary_part = ((lhs.real_part * rhs.imaginary_part) + (lhs.imaginary_part * rhs.real_part));

        return result;
    }

    ComplexNumber operator/(ComplexNumber lhs, ComplexNumber rhs) {
        ComplexNumber result = new ComplexNumber();

        int64 denominator = (rhs.real_part * rhs.real_part) + (rhs.imaginary_part * rhs.imaginary_part);

        result.real_part = ((lhs.real_part * rhs.real_part) + (lhs.imaginary_part * rhs.imaginary_part));
        result.real_part /= denominator;

        result.imaginary_part = ((lhs.imaginary_part * rhs.real_part) - (lhs.real_part * rhs.imaginary_part));
        result.imaginary_part /= denominator;

        return result;
    }
}