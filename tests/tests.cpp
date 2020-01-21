#include "../src/include/Stop_Bus_Parsers.h"

#include <gtest/gtest.h>

TEST(Stops, GetStopName){

    std::string StopName = "Marushkino";
    Stop s(StopName, 55.595884, 37.209755);
    ASSERT_EQ("Marushkino", s.GetStopName());
}



