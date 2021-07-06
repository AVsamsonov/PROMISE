//
// Created by Alex Samsonov on 6/06/21.
//

#include "gtest/gtest.h"
#include "../../Core/Network.h"

using namespace PROMISE::Core;

TEST(Promise, Network) {
    auto defaultNodeData = NodeData(Network::getTypeIdentifier());
    Network::initDefaultNodeData(defaultNodeData);
    EXPECT_TRUE(defaultNodeData.isValid());
    ASSERT_TRUE(defaultNodeData.hasProperty("Network::Connections"));
    EXPECT_EQ(defaultNodeData.getProperty("Network::Connections"), juce::Array<var>());

    auto net = Network(defaultNodeData);
    EXPECT_TRUE(net.isValid());
    EXPECT_FALSE(net.getParent().isValid());
    EXPECT_EQ(net.getType(), Network::getTypeIdentifier());
    EXPECT_TRUE(net.isNetwork());

    EXPECT_EQ(net.getChildren().size(), 0);
    auto node = net.createNode("Node");
    EXPECT_EQ(net.getChildren().size(), 1);
    EXPECT_EQ(node.getName(), "Node");
    auto node1 = net.createNode("Node");
    EXPECT_EQ(net.getChildren().size(), 2);
    EXPECT_EQ(node1.getName(), "Node1");
    auto myNode = net.createNode("Node", "myNode");
    EXPECT_EQ(net.getChildren().size(), 3);
    EXPECT_EQ(myNode.getName(), "myNode");
    EXPECT_THROW(net.createNode("foo"), std::runtime_error);
    EXPECT_EQ(net.getChildren().size(), 3);

    node = net.getNode("myNode");
    ASSERT_TRUE(node.isValid());
    EXPECT_EQ(node.getName(), "myNode");

    node = net.getNode("foo");
    EXPECT_FALSE(node.isValid());
}

