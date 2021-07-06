//
// Created by Alex Samsonov on 21/06/21.
//

#include "gtest/gtest.h"
#include "../../Core/RacksNet.h"
#include "../../Core/Rack.h"

using namespace PROMISE::Core;

TEST(Promise, RacksNodesIn) {
    auto defaultNodeData = NodeData(RacksNet::getTypeIdentifier());
    Node::initDefaultNodeData(defaultNodeData);
    EXPECT_TRUE(defaultNodeData.isValid());

    auto racks = RacksNet(defaultNodeData);
    EXPECT_TRUE(racks.isValid());
    EXPECT_FALSE(racks.getParent().isValid());
    EXPECT_EQ(racks.getType(), RacksNet::getTypeIdentifier());
    EXPECT_TRUE(racks.isNetwork());

    auto rack1 = static_cast<Rack>(racks.createNode("Rack", "rack1"));
    ASSERT_EQ(racks.getChildren().size(), 1);
    EXPECT_EQ(rack1.getName(), "rack1");

    auto in = rack1.createNode("In");
    EXPECT_EQ(in.getParent(), rack1);
    EXPECT_EQ(in.getType().toString(), "In");
    EXPECT_EQ(in.getNumInputPlugs(), 0);
    EXPECT_EQ(in.getNumOutputPlugs(), 2);
    EXPECT_EQ(in.getOutputPlugName(0), "Midi");
    EXPECT_EQ(in.getOutputPlugDataType(0), Node::PlugDataType::Midi);
    EXPECT_EQ(in.getOutputPlugName(1), "Audio");
    EXPECT_EQ(in.getOutputPlugDataType(1), Node::PlugDataType::Audio);
    EXPECT_THROW(in.getOutputPlugName(2), std::runtime_error);
    EXPECT_THROW(in.getOutputPlugDataType(2), std::runtime_error);
    EXPECT_THROW(in.getInputPlugName(0), std::runtime_error);
    EXPECT_THROW(in.getInputPlugDataType(0), std::runtime_error);
}

