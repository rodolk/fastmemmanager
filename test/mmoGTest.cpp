#include <iostream>
#include <gtest/gtest.h>
#include <memoryManager.h>

using namespace objmem_manager;

TEST(MemoryManagerBasic, UsageTest) {
    MemoryManager<int> mmInt(5);
    int *pvalue = mmInt.createInstance();

    ASSERT_NE(pvalue, nullptr);

    *pvalue = 5;

    mmInt.returnInstance(pvalue);

    ASSERT_NO_THROW(pvalue = mmInt.createInstance());

    mmInt.returnInstance(pvalue);

    ASSERT_NE(pvalue, nullptr);
}

TEST(MemoryManagerBasic, OutOfObjectTest) {
    MemoryManager<int> mmInt2(5);
    int *pvalue;
    int i;
    for(i = 0; i < 5; i++) {
        pvalue = mmInt2.createInstance();
        ASSERT_NE(pvalue, nullptr);
        *pvalue = i;
    }

    ASSERT_THROW(mmInt2.createInstance(), std::bad_alloc);
}

TEST(MemoryManagerBasic, Zero) {
    ASSERT_THROW(MemoryManager<int> mmInt(0), std::bad_alloc);
}

TEST(MemoryManagerBasic, OneInstance) {
    MemoryManager<int> mmInt2(1);
    ASSERT_NO_THROW(mmInt2.createInstance());
    ASSERT_THROW(mmInt2.createInstance(), std::bad_alloc);
}
class TestClass {
public:
    TestClass() {}
    void setValue(int v) {value = v;}
    int getValue() {return value;}
private:
    int value{0};
};

TEST(MemoryManagerObjectBasic, UsageTest) {
    MemoryManager<TestClass> mmTestClass(5);
    TestClass *ptr = mmTestClass.createInstance();
    ASSERT_NE(ptr, nullptr);

    ptr->setValue(348);
    ASSERT_EQ(ptr->getValue(), 348);

    mmTestClass.returnInstance(ptr);

    TestClass *ptr2;
    ASSERT_NO_THROW(ptr2 = mmTestClass.createInstance());
    //We can do this because I know the implementation but one day it'll fail
    ASSERT_NE(ptr2, nullptr);
    ASSERT_EQ(ptr2->getValue(), 348);

    mmTestClass.returnInstance(ptr);
}


TEST(MemoryManagerObjectBasic, OutOfObjectTest) {
    MemoryManager<TestClass> mmTestClass(5);
    TestClass *ptr{nullptr};
    int i;
    for(i = 0; i < 5; i++) {
        ptr = mmTestClass.createInstance();
        ASSERT_NE(ptr, nullptr);
        ptr->setValue(i);
    }

    ASSERT_THROW(mmTestClass.createInstance(), std::bad_alloc);
}

TEST(MemoryManagerObjectBasic, HundredTimes) {
    MemoryManager<TestClass> mmTestClass(5);
    TestClass *ptr[5];
    int i;
    int j;
    for(j = 0; j < 100; j++) {
        for(i = 0; i < 5; i++) {
            ptr[i] = mmTestClass.createInstance();
            ASSERT_NE(ptr[i], nullptr);
            ptr[i]->setValue(i);
        }
        for(i = 0; i < 5; i++) {
            mmTestClass.returnInstance(ptr[i]);
        }
    }

    ASSERT_NO_THROW(mmTestClass.createInstance());
}


TEST(MemoryManagerObjectIncrement, SimpleUsageTest) {
    MemoryManager<TestClass> mmTestClass(5, 2, 3);
    TestClass *ptr = mmTestClass.createInstance();
    ASSERT_NE(ptr, nullptr);

    ptr->setValue(348);
    ASSERT_EQ(ptr->getValue(), 348);

    mmTestClass.returnInstance(ptr);

    TestClass *ptr2;
    ASSERT_NO_THROW(ptr2 = mmTestClass.createInstance());
    //We can do this because I know the implementation but one day it'll fail
    ASSERT_NE(ptr2, nullptr);
    ASSERT_EQ(ptr2->getValue(), 348);

    mmTestClass.returnInstance(ptr);
}

TEST(MemoryManagerObjectIncrement, IncrementUsageTest) {
    MemoryManager<TestClass> mmTestClass(5, 2, 3);
    TestClass *ptr{nullptr};
    int i;
    for(i = 0; i < 5; i++) {
        ptr = mmTestClass.createInstance();
        ASSERT_NE(ptr, nullptr);
        ptr->setValue(i);
    }
    for(i = 0; i < 6; i++) {
        ASSERT_NO_THROW(ptr = mmTestClass.createInstance());
        ASSERT_NE(ptr, nullptr);
        ptr->setValue(i);
    }
    ASSERT_THROW(mmTestClass.createInstance(), std::bad_alloc);
}

TEST(MemoryManagerObjectIncrement, HundredTimes) {
    MemoryManager<TestClass> mmTestClass(5, 2, 3);
    TestClass *ptr[11];
    TestClass *ptr2 = nullptr;
    int i;
    int j;
    for(j = 0; j < 100; j++) {
        for(i = 0; i < 11; i++) {
            ASSERT_NO_THROW(ptr[i] = mmTestClass.createInstance());
            ASSERT_NE(ptr[i], nullptr);
            ptr[i]->setValue(i);
        }
        ASSERT_THROW(mmTestClass.createInstance(), std::bad_alloc);
        for(i = 0; i < 11; i++) {
            mmTestClass.returnInstance(ptr[i]);
        }
    }

    ASSERT_NO_THROW(ptr2 = mmTestClass.createInstance());
    ASSERT_NE(ptr2, nullptr);
    mmTestClass.returnInstance(ptr2);
}

TEST(MemoryManagerObjectIncrement, HundredTimesBig) {
    const int initial = 1000;
    const int increm = 2000;
    const int maxIncrem = 3;
    const int totObjs = initial + (increm * maxIncrem);
    MemoryManager<TestClass> mmTestClass(initial, increm, maxIncrem);
    TestClass *ptr[totObjs];
    TestClass *ptr2 = nullptr;
    int i;
    int j;
    for(j = 0; j < 100; j++) {
        for(i = 0; i < totObjs; i++) {
            ASSERT_NO_THROW(ptr[i] = mmTestClass.createInstance());
            ASSERT_NE(ptr[i], nullptr);
            ptr[i]->setValue(i);
        }
        ASSERT_THROW(mmTestClass.createInstance(), std::bad_alloc);
        for(i = 0; i < totObjs; i++) {
            mmTestClass.returnInstance(ptr[i]);
        }
    }

    ASSERT_NO_THROW(ptr2 = mmTestClass.createInstance());
    ASSERT_NE(ptr2, nullptr);
    mmTestClass.returnInstance(ptr2);
}

TEST(MemoryManagerObjectIncrement, IncrementZero) {
    const int initial = 1000;
    const int increm = 2000;
    const int maxIncrem = 0;
    const int totObjs = initial + (increm * maxIncrem);
    MemoryManager<TestClass> mmTestClass(initial, increm, maxIncrem);
    TestClass *ptr[initial];
    int i;
    for(i = 0; i < initial; i++) {
        ASSERT_NO_THROW(ptr[i] = mmTestClass.createInstance());
        ASSERT_NE(ptr[i], nullptr);
        ptr[i]->setValue(i);
    }
    ASSERT_THROW(mmTestClass.createInstance(), std::bad_alloc);
    for(i = 0; i < totObjs; i++) {
        mmTestClass.returnInstance(ptr[i]);
    }
}

