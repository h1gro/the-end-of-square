#include <TXLib.h>
#include <stdio.h>
#include <math.h>

/**
 * @brief Tests Structure for tests
 * @param a square equation coefficient
 * @param b square equation coefficient
 * @param ñ square equation coefficient
 * @param x1exp the first expected root
 * @param x2exp the second expected root
 * @param nRootsExp the expected number of roots
 */
struct Tests
{
    double a, b, c;
    double x1exp, x2exp;
    int nRootsExp;
};

/**
 * @brief Find roots of square equation with
 * coefficients a, b, c and the number of roots
 * @return Return the number of roots
 * @param a square equation coefficient
 * @param b square equation coefficient
 * @param ñ square equation coefficient
 * @param x1 the first root
 * @param x2 the second root
 */
int SolveSquare(double a, double b, double c, double* x1, double* x2);

/**
 * @brief Open the test from file and scan tests from it
 * @return Return results of all tests
 */
int ScanTestsFile();

/**
 * @brief Compare roots from functions SolveSquare and SolveLinear with expected roots
 * @return Return the result of the test
 * @param nTest the number of the test
 * @param struct_test structure where keeps coefficients
 * a, b, c, roots x1exp, x2exp and nRootsExp
 */
int Tester(struct Tests struct_test, int nTest);

/**
 * @brief Find roots of linear equation with
 * coefficients b, c and number of roots
 * @return Return the number of roots
 * @param b linear equation coefficient
 * @param ñ linear equation coefficient
 * @param x1 first root
 */
int SolveLinear(double b, double c, double* x1);

/**
 * @brief Print the result of the programm
 * @param nRoots the number of roots
 * @param x1 first root
 * @param x2 second root
 */
void PrintResult(int nRoots, double x1, double x2);

/**
 * @brief Clear buffer from wrong coefficients
 */
void ClearBuffer();

/**
 * @brief Compare abs difference between two elements and epsilon
 * @return Return True or False
 * @param elem1 the first element
 * @param elem2 the second element
 */
bool IsEqual(double elem1, double elem2);

/**
 * @brief Number_of_roots enumeration of constants that mean number of roots
 * @param NO_ROOTS the number of roots is zero
 * @param ONE_ROOT the number of roots is one
 * @param TWO_ROOTS the number of roots is two
 * @param INFINITY_ROOTS the number of roots is infinity
 */
enum Number_of_roots
{
    NO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INFINITY_ROOTS = 3,
};

/**
 * @brief Tests_results enumeration of constants that mean results of tests
 * @param TEST_DONE the test has gone successfully
 * @param TEST_FAILED the test has gone unsuccessfully
 * @param ALL_TESTS_DONE all test have gone successfully
 * @param INCORRECT_TEST the test is not correct
 * @param FILE_NOT_OPENED the file is not opened
 */
enum Tests_results
{
    TEST_DONE = 4,
    TEST_FAILED = 5,
    ALL_TESTS_DONE = 6,
    INCORRECT_TEST = 7,
    FILE_NOT_OPENED = 8,
};
/**
 * @param EPSILON the minimum difference between two
 * elements in which that two elements can be equal
 */
const double EPSILON = 0.0001;

int main()
{
    double a = 0, b = 0, c = 0, x1 = 0, x2 = 0;

    int identificator = ScanTestsFile();

    if (identificator == ALL_TESTS_DONE)
    {
        while (scanf("%lg %lg %lg", &a, &b, &c) != 3)
        {
            printf("wrong input");
            ClearBuffer();
        }

        int nRoots = SolveSquare (a, b, c, &x1, &x2);
        PrintResult(nRoots, x1, x2);
    }
}


int SolveSquare ( double a, double b, double c, double* x1, double* x2)
{
    assert(x1);
    assert(x2);

    double D = b * b - 4 * a * c;

    if ( IsEqual(a, 0) )
    {
        return SolveLinear(b, c, x1);
    }
    else
    {
        if (D >= 0)
        {
            double sqrt_D = sqrt(D);
            *x1 = (-b + sqrt_D) / (2 * a);
            *x2 = (-b - sqrt_D) / (2 * a);
            if ( IsEqual(*x1, *x2) )
            {
                return ONE_ROOT;
            }
            else
            {
                return TWO_ROOTS;
            }
        }
        else
        {
            return NO_ROOTS;
        }
    }
}

int SolveLinear(double b, double c, double* x1)
{
        assert(x1);

        if ( IsEqual(b, 0) )
        {
            if ( IsEqual(c, 0) )
            {
                return INFINITY_ROOTS;
            }
            else
            {
                return NO_ROOTS;
            }
        }
        else
        {
            *x1 = -c / b;
            return ONE_ROOT;
        }
}

void ClearBuffer()
{
    int ch = getchar();
    while (ch != EOF && ch != '\n' )
    {
        ch = getchar();
    }
}

int Tester(struct Tests struct_test, int nTest)
{

    double x1 = 0, x2 = 0;

    int nRoots = SolveSquare(struct_test.a, struct_test.b, struct_test.c, &x1, &x2);

    if (nRoots != struct_test.nRootsExp
        || !IsEqual(x1, struct_test.x1exp)
        || !IsEqual(x2, struct_test.x2exp))
    {
        printf("\n" "error test = %d, a = %lg, b = %lg, c = %lg\n"
               " x1 = %lg, x2 = %lg, nRoots = %d\n"
               "expected, x1exp = %lg, x2exp = %lg, nRootsExp = %d\n"
               "x1-x1exp = %lg, x2-x2exp= %lg\n\n",
               nTest, struct_test.a, struct_test.b, struct_test.c,
               x1, x2, nRoots,
               struct_test.x1exp, struct_test.x2exp, struct_test.nRootsExp,
               x1-struct_test.x1exp, x2-struct_test.x2exp);

        return TEST_FAILED;
    }
    else
    {
        printf("Successfully\n");

        return TEST_DONE;
    }
}

int ScanTestsFile()
{
    struct Tests struct_test = {};
    int nTest = 0;

    FILE * file = fopen("tests.txt", "r");
    if (file == NULL)
    {
        printf("file not opened");
        return FILE_NOT_OPENED;
    }

    while( fscanf(file,
                  "%lg %lg %lg %lg %lg %d",
                    &struct_test.a, &struct_test.b, &struct_test.c,
                    &struct_test.x1exp, &struct_test.x2exp, &struct_test.nRootsExp) == 6)
    {
        int check = 0;

        check = Tester(struct_test, nTest);

        if (check == TEST_FAILED)
        {
            fclose(file);
            return INCORRECT_TEST;
        }

        nTest++;
    }
    fclose(file);
    return ALL_TESTS_DONE;
}

bool IsEqual(double elem1, double elem2)
{
    return (fabs(elem1 - elem2) <= EPSILON);
}

void PrintResult(int nRoots, double x1, double x2)
{
    switch(nRoots)
    {
        case NO_ROOTS:       printf("no roots\n");
                             break;

        case ONE_ROOT:       printf("%lg\n", x1);
                             break;

        case TWO_ROOTS:      printf("%lg %lg\n", x1, x2);
                             break;

        case INFINITY_ROOTS: printf("any number");
                             break;

        default:             printf("unreachable");
                             break;

    }

}


