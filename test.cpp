#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void printMatrix(float* mat, int rowCount, int colCount);

int main()
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    printMatrix(&modelMatrix[0][0], 4, 4);
    
    return 0;
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