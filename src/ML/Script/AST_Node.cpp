#include <ML/Script/AST_Node.hpp>
#include <ML/Script/AST_Block.hpp>
#include <ML/Script/AST_Expr.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	int32_t AST_Node::s_id = 0;

	/* * * * * * * * * * * * * * * * * * * * */

	AST_Node::AST_Node()
		: m_parent(nullptr)
		, m_children()
		, m_id(++s_id)
	{
	}

	AST_Node::AST_Node(const List<AST_Node *> & children)
		: m_parent(nullptr)
		, m_children(children)
		, m_id(++s_id)
	{
		for (auto e : m_children)
		{
			addChild(e);
		}
	}

	AST_Node::AST_Node(const AST_Node & copy)
		: m_parent(copy.m_parent)
		, m_children(copy.m_children)
		, m_id(copy.m_id)
	{
	}

	AST_Node::~AST_Node()
	{
		cleanChildren();
		clearChildren();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const List<AST_Node *> & AST_Node::getChildren() const
	{
		return m_children;
	}

	const List<AST_Node *> & AST_Node::getSiblings() const
	{
		static List<AST_Node *> empty;

		if (AST_Node * n = getParent())
		{
			return n->getChildren();
		}

		return empty;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void AST_Node::clearChildren()
	{
		m_children.clear();
	}

	void AST_Node::cleanChildren()
	{
		for (AST_Node * child : m_children)
		{
			delete child;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool AST_Node::empty() const
	{
		return m_children.empty();
	}
		 
	bool AST_Node::contains(AST_Node * node) const
	{
		return find(node) != end();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	size_t	AST_Node::getDepth() const
	{
		if (auto p = getParent())
		{
			return p->getDepth() + 1;
		}
		return 0;
	}

	size_t	AST_Node::childCount() const
	{
		return m_children.size();
	}

	size_t	AST_Node::siblingCount() const
	{
		return getSiblings().size();
	}

	size_t	AST_Node::siblingIndex() const
	{
		if (AST_Node * p = getParent())
		{
			size_t i = 0;

			for (AST_Node * c : (*p))
			{
				if (c == this)
				{
					return i;
				}

				i++;
			}
		}

		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	AST_Node * AST_Node::addChild(AST_Node * value)
	{
		if (value)
		{
			value->setParent(this);
			if (std::find(begin(), end(), value) == end())
			{
				m_children.push_back(value);
			}
			return value;
		}
		return nullptr;
	}

	AST_Node * AST_Node::getChild(size_t index) const
	{
		if (index < childCount())
		{
			return m_children.at(index);
		}
		return nullptr;
	}

	AST_Node * AST_Node::getParent() const
	{
		return m_parent;
	}

	AST_Node * AST_Node::getSibling(size_t index) const
	{
		List<AST_Node *> siblings = getSiblings();
		if (index < siblings.size())
		{
			return siblings[index];
		}
		return nullptr;
	}

	AST_Node * AST_Node::push_front(AST_Node * value)
	{
		return insertChild(0, value);
	}

	AST_Node * AST_Node::insertChild(size_t index, AST_Node * value)
	{
		if (value)
		{
			value->setParent(this);
			m_children.insert(begin() + index, value);
			return value;
		}
		return nullptr;
	}

	AST_Node * AST_Node::insertChildAfter(AST_Node * pos, AST_Node * value)
	{
		if (value)
		{
			AST_Node::const_iterator it;
			if ((it = find(pos)) != end())
			{
				value->setParent(this);
				m_children.insert(it + 1, value);
				return value;
			}
		}
		return nullptr;
	}

	AST_Node * AST_Node::insertChildBefore(AST_Node * pos, AST_Node * value)
	{
		if (value)
		{
			AST_Node::const_iterator it;
			if ((it = find(pos)) != end())
			{
				value->setParent(this);
				m_children.insert(it, value);
			}
			return value;
		}
		return nullptr;
	}

	AST_Node * AST_Node::getNext() const
	{
		size_t i = siblingIndex() + 1;
		if (i < siblingCount())
		{
			return getSibling(i);
		}
		return nullptr;
	}

	AST_Node * AST_Node::getPrev() const
	{
		size_t i = siblingIndex();
		if (i > 0)
		{
			return getSibling(i - 1);
		}
		return nullptr;
	}

	AST_Node * AST_Node::setParent(AST_Node * value)
	{
		m_parent = value;

		return m_parent;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	ostream & AST_Node::display(ostream & out) const
	{
		for (AST_Node * n : (*this))
		{
			n->display(out);
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	int32_t	AST_Node::getID() const
	{
		return m_id;
	}

	bool	AST_Node::run()
	{
		return runNext();
	}

	bool	AST_Node::runNext()
	{
		if (AST_Node * n = getNext())
		{
			return n->run();
		}
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}