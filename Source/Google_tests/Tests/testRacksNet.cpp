//
// Created by Alex Samsonov on 6/06/21.
//

#include "gtest/gtest.h"
#include "../../Core/RacksNet.h"

using namespace PROMISE::Core;

TEST(Promise, RacksNet) {
    auto defaultNodeData = NodeData(RacksNet::getTypeIdentifier());
    Node::initDefaultNodeData(defaultNodeData);
    EXPECT_TRUE(defaultNodeData.isValid());

    auto racks = RacksNet(defaultNodeData);
    EXPECT_TRUE(racks.isValid());
    EXPECT_FALSE(racks.getParent().isValid());
    EXPECT_EQ(racks.getType(), RacksNet::getTypeIdentifier());
    EXPECT_TRUE(racks.isNetwork());

    auto rack1 = racks.createNode("Rack", "rack1");
    ASSERT_EQ(racks.getChildren().size(), 1);
    EXPECT_EQ(rack1.getName(), "rack1");
}