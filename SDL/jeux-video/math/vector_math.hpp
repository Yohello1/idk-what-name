namespace vector_meth
{
    struct vector_3d
    {
        uint8_t x;
        uint8_t y;
        uint8_t z;
        uint8_t u;
    };

    vector_3d add(vector_3d point1, vector_3d point2)
    {
        vector_3d return_val;
        return_val.x = point1.x + point2.x;
        return_val.y = point1.y + point2.y;
        return_val.z = point1.z + point2.z;
        return_val.u = point1.u + point2.u;
        return return_val;
    }

    vector_3d subtract(vector_3d point1, vector_3d point2)
    {
        vector_3d return_val;
        return_val.x = point1.x - point2.x;
        return_val.y = point1.y - point2.y;
        return_val.z = point1.z - point2.z;
        return_val.u = point1.u - point2.u;
        return return_val;
    }

    vector_3d average(vector_3d point1, vector_3d point2)
    {
        vector_3d return_val;
        return_val.x = (point1.x - point2.x) / 2;
        return_val.y = (point1.y - point2.y) / 2;
        return_val.z = (point1.z - point2.z) / 2;
        return_val.u = (point1.u - point2.u) / 2;
        return return_val;
    }

    vector_3d multiply(vector_3d point1, double_t scalar_val)
    {
        vector_3d return_val;
        return_val.x = point1.x * scalar_val;
        return_val.y = point1.y * scalar_val;
        return_val.z = point1.z * scalar_val;
        return_val.u = point1.u * scalar_val;
        return return_val;
    }

    vector_3d divde(vector_3d point1, double_t scalar_val)
    {
        vector_3d return_val;
        return_val.x = point1.x * scalar_val;
        return_val.y = point1.y * scalar_val;
        return_val.z = point1.z * scalar_val;
        return_val.u = point1.u * scalar_val;
        return return_val;
    }
}