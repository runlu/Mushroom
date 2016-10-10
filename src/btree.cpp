/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:   https://www.github.com/UncP/Mushroom
 *    > Description:
 *
 *    > Created Time: 2016-10-07 20:12:13
**/

#include <cassert>

#include "btree.hpp"

namespace Mushroom {

std::string BTree::ToString() const
{
	std::string str;
	char no[16];
	snprintf(no, 16, "%d  ", degree_);
	str = "阶: " + std::string(no);
	snprintf(no, 16, "%d  ", min_key_);
	str = "min_key: " + std::string(no);
	snprintf(no, 16, "%d  ", max_key_);
	str = "max_key: " + std::string(no);
	snprintf(no, 16, "%d  ", min_node_);
	str = "min_node: " + std::string(no);
	snprintf(no, 16, "%d  ", max_node_);
	str = "max_node: " + std::string(no) + "\n";
	return std::move(str);
}

Status BTree::Init(const int fd, const int key_len)
{
	// TODO

	key_len_ = static_cast<uint8_t>(key_len);

	degree_ = static_cast<uint16_t>(
		(BTreePage::PageSize - sizeof(BTreePage)) /
		(DataPage::DataId + BTreePage::IndexByte + key_len) + 1);

	pager_ = new BTreePager(fd);

	return Success;
}

Status BTree::Close()
{
	if (pager_) assert(pager_->Close());
	return Success;
}

BTreePage* BTree::DescendToLeaf(const Slice &key, BTreePage **stack, uint8_t *depth)
{
	BTreePage *node = &root_;
	for (; node->Type() != BTreePage::LEAF; ++*depth) {
		page_id page_no = node->Descend(key);
		stack[*depth] = node;
		assert(node = pager_->GetPage(page_no));
	}
	return node;
}

Status BTree::Put(const Slice &key, const Slice &val)
{
	assert(key.Length() == key_len_);
	assert(val.Empty());

	uint8_t depth = 0;
	BTreePage* stack[8];

	BTreePage *leaf = DescendToLeaf(key, stack, &depth);
	leaf->Insert(key);

	return Success;
}

} // namespace Mushroom
