#ifndef BERNSTEIN_H
#define BERNSTEIN_H

#include <array>
#include <cmath>

class Bernstein
{
public :
    Bernstein() 
    {
        InitializeData();
    }
    float Value(double t, int i, int n)
    {
        return binomialCoeff[n][i] * (float)pow(t, i) * (float)pow(1 - t, n - i);
    }
private:
    void InitializeData()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = i; j < 4; j++)
            {
                binomialCoeff[i][j] = -1;
            }
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = i; j < 4; j++)
            {
                if (i == 0)
                {
                    binomialCoeff[j][i] = 1;
                }
                else if (i == j)
                {
                    binomialCoeff[j][i] = 1;
                }
                else
                {
                    binomialCoeff[j][i] = binomialCoeff[j - 1][i - 1] + binomialCoeff[j - 1][i];
                }
            }
        }
    }
	std::array<std::array<int, 4>, 4> binomialCoeff;
};

#endif // !BERNSTEIN_H
