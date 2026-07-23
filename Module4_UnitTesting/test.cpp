#include "pch.h"

#include <cstdlib>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <vector>

// Global Google Test environment used to initialize the random number seed.
class Environment : public testing::Environment
{
public:
    Environment() = default;

    void SetUp() override
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    void TearDown() override
    {
    }
};

// Test fixture that creates a new vector before each test and releases it afterward.
class CollectionTest : public testing::Test
{
protected:
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    {
        collection = std::make_unique<std::vector<int>>();
    }

    void TearDown() override
    {
        collection->clear();
        collection.reset();
    }

    // Adds the requested number of random values from 0 through 99.
    void add_entries(int count)
    {
        ASSERT_GT(count, 0);

        for (int i = 0; i < count; ++i)
        {
            collection->push_back(std::rand() % 100);
        }
    }
};

// Verifies that the collection smart pointer is created successfully.
TEST_F(CollectionTest, CollectionStartPointerIsNotNull)
{
    ASSERT_NE(collection.get(), nullptr);
}

// Verifies that a newly created collection contains no values.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    ASSERT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0U);
}

// Verifies that one value can be added to an empty collection.
TEST_F(CollectionTest, CanAddOneValueToEmptyCollection)
{
    ASSERT_TRUE(collection->empty());

    add_entries(1);

    EXPECT_FALSE(collection->empty());
    EXPECT_EQ(collection->size(), 1U);
}

// Verifies that five values can be added to the collection.
TEST_F(CollectionTest, CanAddFiveValuesToCollection)
{
    add_entries(5);

    EXPECT_EQ(collection->size(), 5U);
}

// Verifies that max_size is never smaller than the current collection size.
TEST_F(CollectionTest, MaxSizeIsAtLeastCurrentSize)
{
    EXPECT_GE(collection->max_size(), collection->size());

    add_entries(1);
    EXPECT_GE(collection->max_size(), collection->size());

    add_entries(4);
    EXPECT_GE(collection->max_size(), collection->size());

    add_entries(5);
    EXPECT_GE(collection->max_size(), collection->size());
}

// Verifies that capacity is never smaller than the current collection size.
TEST_F(CollectionTest, CapacityIsAtLeastCurrentSize)
{
    EXPECT_GE(collection->capacity(), collection->size());

    add_entries(1);
    EXPECT_GE(collection->capacity(), collection->size());

    add_entries(4);
    EXPECT_GE(collection->capacity(), collection->size());

    add_entries(5);
    EXPECT_GE(collection->capacity(), collection->size());
}

// Verifies that resize can increase and decrease the collection size.
TEST_F(CollectionTest, ResizeChangesCollectionSize)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5U);

    collection->resize(10);
    EXPECT_EQ(collection->size(), 10U);

    collection->resize(3);
    EXPECT_EQ(collection->size(), 3U);
}

// Verifies that resize can reduce the collection size to zero.
TEST_F(CollectionTest, ResizeCanReduceCollectionToZero)
{
    add_entries(5);
    ASSERT_FALSE(collection->empty());

    collection->resize(0);

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0U);
}

// Verifies that clear removes every value from the collection.
TEST_F(CollectionTest, ClearRemovesAllValues)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5U);

    collection->clear();

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0U);
}

// Verifies that erasing the complete iterator range removes all values.
TEST_F(CollectionTest, EraseEntireRangeRemovesAllValues)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5U);

    collection->erase(collection->begin(), collection->end());

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0U);
}

// Verifies that reserve increases capacity without changing collection size.
TEST_F(CollectionTest, ReserveIncreasesCapacityWithoutChangingSize)
{
    add_entries(5);

    const std::size_t originalSize = collection->size();
    const std::size_t requestedCapacity = collection->capacity() + 10U;

    collection->reserve(requestedCapacity);

    EXPECT_EQ(collection->size(), originalSize);
    EXPECT_GE(collection->capacity(), requestedCapacity);
}

// Custom positive test: verifies that push_back stores the expected value.
TEST_F(CollectionTest, PushBackStoresExpectedValue)
{
    const int expectedValue = 42;

    collection->push_back(expectedValue);

    ASSERT_EQ(collection->size(), 1U);
    EXPECT_EQ(collection->front(), expectedValue);
    EXPECT_EQ(collection->back(), expectedValue);
}

// Custom negative test: verifies that invalid access throws an exception.
TEST_F(CollectionTest, AccessingInvalidIndexThrowsOutOfRange)
{
    ASSERT_TRUE(collection->empty());

    EXPECT_THROW(
        static_cast<void>(collection->at(0)),
        std::out_of_range);
}