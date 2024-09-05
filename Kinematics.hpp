#include <Arduino.h>
#include <math.h>


class Kinematics {
  
  public:

    struct arrWrap1 {
      float arr[3][1];
    };

    struct arrWrap3 {
      float arr[3][3];
    };

    struct arrWrap4 {
      float arr[4][4];
    };


    // inverse kinematics
    struct arrWrap1 getAngles(float end_x, float end_y, float end_z, float link_len1, float link_len2, float base_height);

    // print matrix
    void printMatrix(HardwareSerial &serial, String title, struct arrWrap1 pose);
    void printMatrix(HardwareSerial &serial, String title, struct arrWrap3 pose);
    void printMatrix(HardwareSerial &serial, String title, struct arrWrap4 pose);

  private: 

    const struct arrWrap3 identityMatrix = {{
        {1, 0, 0}, 
        {0, 1, 0}, 
        {0, 0, 1}
    }};

    // homogenous transformation matrix
    struct arrWrap4 getHTM(struct arrWrap3 rotationMatrix, struct arrWrap1 displacementVector);

    // rotation be about given axis
    struct arrWrap3 xRotation(float angle);
    struct arrWrap3 yRotation(float angle);
    struct arrWrap3 zRotation(float angle);
    
    // unit conversion
    float toRadians(float degrees);
    float toDegrees(float radians);

    // multiply two matrices
    struct arrWrap3 multiply(struct arrWrap3 matrixA, struct arrWrap3 matrixB);
    struct arrWrap4 multiply(struct arrWrap4 matrixA, struct arrWrap4 matrixB);

};