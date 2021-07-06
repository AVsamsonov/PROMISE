//
// Created by Alex Samsonov on 14/06/21.
//

#include "gtest/gtest.h"
#include "../../Core/SongNet.h"

using namespace PROMISE::Core;

TEST(Promise, SongNet) {
    auto defaultNodeData = NodeData(SongNet::getTypeIdentifier());
    SongNet::initDefaultNodeData(defaultNodeData);
    EXPECT_TRUE(defaultNodeData.isValid());

    auto songNet = SongNet(defaultNodeData);
    EXPECT_TRUE(songNet.isValid());
    EXPECT_FALSE(songNet.getParent().isValid());
    EXPECT_EQ(songNet.getType(), SongNet::getTypeIdentifier());
    EXPECT_TRUE(songNet.isNetwork());

    ASSERT_EQ(songNet.getChildren().size(), 1);
    auto racks = songNet.getNode("Racks");
    ASSERT_TRUE(racks.isValid());
}