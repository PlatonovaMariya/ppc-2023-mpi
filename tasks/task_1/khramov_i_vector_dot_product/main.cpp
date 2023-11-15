// Copyright 2023 Khramov Ivan
#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/vector.hpp>
#include "./vector_dot_product.h"
#include <boost/mpi.hpp>


TEST(Vector_Dot_Product_MPI, Test_Equal_Size_Vectors) {
    boost::mpi::communicator world;
    std::vector<int> global_vec1, global_vec2;
    const int count_size_vector = 500;

    if (world.rank() == 0) {
        global_vec1 = getRandomVector(count_size_vector);
        global_vec2 = getRandomVector(count_size_vector);
    }

    boost::mpi::broadcast(world, global_vec1, 0);
    boost::mpi::broadcast(world, global_vec2, 0);

    int global_product = getVectorDotProduct(world, global_vec1, global_vec2);

    if (world.rank() == 0) {
        int reference_product = std::inner_product(
                                    std::begin(global_vec1),
                                    std::end(global_vec1),
                                    std::begin(global_vec2), 0);
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(Vector_Dot_Product_MPI, Test_Not_Equal_Size_Vectors) {
    boost::mpi::communicator world;
    std::vector<int> global_vec1, global_vec2;
    const int count_size_vector = 500;

    if (world.rank() == 0) {
        global_vec1 = getRandomVector(count_size_vector);
        global_vec2 = getRandomVector(count_size_vector + 1);
    }

    boost::mpi::broadcast(world, global_vec1, 0);
    boost::mpi::broadcast(world, global_vec2, 0);

    int global_product = getVectorDotProduct(world, global_vec1, global_vec2);

    if (world.rank() == 0) {
        int reference_product = -1;
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(Vector_Dot_Product_MPI, Test_One_Size_Vectors) {
    boost::mpi::communicator world;
    std::vector<int> global_vec1, global_vec2;
    const int count_size_vector = 1;

    if (world.rank() == 0) {
        global_vec1 = getRandomVector(count_size_vector);
        global_vec2 = getRandomVector(count_size_vector);
    }

    int global_product = getVectorDotProduct(world, global_vec1, global_vec2);

    if (world.rank() == 0) {
        int reference_product = global_vec1[0] * global_vec2[0];
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(Vector_Dot_Product_MPI, Test_Both_Void_Vectors) {
    boost::mpi::communicator world;
    std::vector<int> global_vec1, global_vec2;
    const int count_size_vector = 0;

    if (world.rank() == 0) {
        global_vec1 = getRandomVector(count_size_vector);
        global_vec2 = getRandomVector(count_size_vector);
    }

    boost::mpi::broadcast(world, global_vec1, 0);
    boost::mpi::broadcast(world, global_vec2, 0);

    int global_product = getVectorDotProduct(world, global_vec1, global_vec2);

    if (world.rank() == 0) {
        int reference_product = 0;
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(Vector_Dot_Product_MPI, Test_First_Void_Vector) {
    boost::mpi::communicator world;
    std::vector<int> global_vec1, global_vec2;
    const int count_size_vector = 0;

    if (world.rank() == 0) {
        global_vec1 = getRandomVector(count_size_vector);
        global_vec2 = getRandomVector(count_size_vector + 1);
    }

    boost::mpi::broadcast(world, global_vec1, 0);
    boost::mpi::broadcast(world, global_vec2, 0);

    int global_product = getVectorDotProduct(world, global_vec1, global_vec2);

    if (world.rank() == 0) {
        int reference_product = -1;
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(Vector_Dot_Product_MPI, Test_Second_Void_Vector) {
    boost::mpi::communicator world;
    std::vector<int> global_vec1, global_vec2;
    const int count_size_vector = 0;

    if (world.rank() == 0) {
        global_vec1 = getRandomVector(count_size_vector + 1);
        global_vec2 = getRandomVector(count_size_vector);
    }

    boost::mpi::broadcast(world, global_vec1, 0);
    boost::mpi::broadcast(world, global_vec2, 0);

    int global_product = getVectorDotProduct(world, global_vec1, global_vec2);

    if (world.rank() == 0) {
        int reference_product = -1;
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(Vector_Dot_Product_MPI, Test_Order_Independent_Vectors) {
    boost::mpi::communicator world;
    std::vector<int> global_vec1, global_vec2;
    const int count_size_vector = 500;

    if (world.rank() == 0) {
        global_vec1 = getRandomVector(count_size_vector);
        global_vec2 = getRandomVector(count_size_vector);
    }

    boost::mpi::broadcast(world, global_vec1, 0);
    boost::mpi::broadcast(world, global_vec2, 0);

    int global_product = getVectorDotProduct(world, global_vec1, global_vec2);

    if (world.rank() == 0) {
        int reference_product = std::inner_product(
                                    std::begin(global_vec2),
                                    std::end(global_vec2),
                                    std::begin(global_vec1), 0);;
        ASSERT_EQ(reference_product, global_product);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    auto& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
