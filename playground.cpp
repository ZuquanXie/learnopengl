#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/mixed_product.hpp>
#include <iostream>
#include <vector>
#include <array>

void printMatrix(float* mat, int rowCount, int colCount);
void printVec(float* vec, int count);
void createVertices(float xLength, float yLength, float zLength, float* vptr);
void createIndices(unsigned int* indices);

int main()
{
    const int count = 12;
    float p[count * 6];
    unsigned int indices[3 * 6];

    createVertices(2.0f, 2.0f, 2.0f, p);
    createIndices(indices);
    float* ptr;
    int i, j;
    i = 0;
    ptr = p;
    while (i < count)
    {
        j = 0;
        while (j < 6)
        {
            std::cout << *ptr++ << " ";
            j++;
        }
        std::cout << std::endl;
        if ((i + 1) % 4 == 0)
            std::cout << std::endl;
        i++;
    }
    unsigned int* ptr2;
    ptr2 = indices;
    i = 0;
    while (i < 3)
    {
        j = 0;
        while (j < 6)
        {
            std::cout << *ptr2++ << " ";
            j++;
        }
        std::cout << std::endl;
        i++;
    }

    return 0;
}

void createIndices(unsigned int* indices)
{
    unsigned int i, s;

    i = 0;
    while (i < 3)
    {
        s = i * 4;
        *indices++ = s;
        *indices++ = s + 1;
        *indices++ = s + 2;
        *indices++ = s + 2;
        *indices++ = s + 3;
        *indices++ = s;
        i++;
    }
}

    void createVertices(float xLength, float yLength, float zLength, float* vptr)
    {
        int i;
        float l;
        float w = 0.1f;
        // x axis
        i = 0;
        l = xLength / 2;
        while (i < 4)
        {
            // x
            if (i % 3 == 0)
                *vptr++ = -l;
            else
                *vptr++ = l;
            // y
            if (i < 2)
                *vptr++ = w;
            else
                *vptr++ = -w;
            // z
            *vptr++ = 0.0f;
            // rgb
            *vptr++ = 1.0f;
            *vptr++ = 0.0f;
            *vptr++ = 0.0f;
            i++;
        }
        // y axis
        i = 0;
        l = yLength / 2;
        while (i < 4)
        {
            // x
            *vptr++ = 0.0f;
            // y
            if (i % 3 == 0)
                *vptr++ = -l;
            else
                *vptr++ = l;
            // z
            if (i < 2)
                *vptr++ = w;
            else
                *vptr++ = -w;
            // rgb
            *vptr++ = 0.0f;
            *vptr++ = 1.0f;
            *vptr++ = 0.0f;
            i++;
        }
        // z axis
        i = 0;
        l = zLength / 2;
        while (i < 4)
        {
            // x
            if (i < 2)
                *vptr++ = w;
            else
                *vptr++ = -w;
            // y
            *vptr++ = 0.0f;
            // z
            if (i % 3 == 0)
                *vptr++ = -l;
            else
                *vptr++ = l;
            // rgb
            *vptr++ = 0.0f;
            *vptr++ = 0.0f;
            *vptr++ = 1.0f;
            i++;
        }
    }

void printMatrix(float* mat, int rowCount, int colCount)
{
    int i, j;

    i = 0;
    while (i < rowCount)
    {
        j = 0;
        while (j < colCount)
        {
            std::cout << *mat << " ";
            mat++;
            j++;
        }
        std::cout << std::endl;
        i++;
    }
}

void printVec(float* vec, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        std::cout << *vec << " ";
        vec++;
        i++;
    }
    std::cout << std::endl;
}