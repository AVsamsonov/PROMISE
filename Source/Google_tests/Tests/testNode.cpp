//
// Created by Alex Samsonov on 1/06/21.
//

#include "gtest/gtest.h"
#include "../../Core/Node.h"
#include "../../Core/Network.h"

using namespace PROMISE::Core;

TEST(Promise, Node) {
    auto defaultNodeData = NodeData(Node::getTypeIdentifier());
    Node::initDefaultNodeData(defaultNodeData);
    EXPECT_TRUE(defaultNodeData.isValid());
    defaultNodeData.hasProperty("Name");

    auto node = Node(defaultNodeData);
    EXPECT_FALSE(node.getParent().isValid());
    EXPECT_TRUE(node.getType().isValid());
    EXPECT_EQ(node.getType(), Node::getTypeIdentifier());
    EXPECT_FALSE(node.isNetwork());
    EXPECT_EQ(node.getName(), "");

    node.setName("myNode");
    EXPECT_EQ(node.getName(), "myNode");
}
