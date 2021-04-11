#include "Transform.h"


namespace SFMLEngine {

		Transform::Transform()
			: m_Position(), m_Rotation(0), m_Scale(sf::Vector2f(1, 1))
		{}
		Transform::Transform(const sf::Vector2f& pos)
			: m_Position(pos), m_Rotation(0), m_Scale(sf::Vector2f(1, 1))
		{}
		Transform::Transform(const sf::Vector2f& pos, float rot, const sf::Vector2f& scale)
			: m_Position(pos), m_Rotation(rot), m_Scale(scale)
		{}

		Transform::Transform(Transform* parent)
			: m_Position(), m_Rotation(0), m_Scale(sf::Vector2f(1, 1)), m_Parent(parent)
		{}
		Transform::Transform(const sf::Vector2f& pos, Transform* parent)
			: m_Position(pos), m_Rotation(0), m_Scale(sf::Vector2f(1, 1)), m_Parent(parent)
		{}


		sf::Transform Transform::GetLocalTransformMatrix() const
		{
			sf::Transform t;
			t.translate(m_Position);
			t.rotate(m_Rotation);
			t.scale(m_Scale);
			return t;
		}

		sf::Transform Transform::GetLocalToWorldTransformMatrix() const
		{
			if (m_Parent)
			{
				return GetLocalTransformMatrix().combine(m_Parent->GetLocalToWorldTransformMatrix());
			}
			else
			{
				return GetLocalTransformMatrix();
			}
		}

		sf::Transform Transform::GetWorldToLocalTransformMatrix() const
		{
			return GetLocalToWorldTransformMatrix().getInverse();
		}

		sf::Vector2f Transform::GetWorldPosition() const
		{
			if (m_Parent)
			{
				return m_Position + m_Parent->GetWorldPosition();
			}
			else
			{
				return m_Position;
			}
		}

		sf::Vector2f Transform::LocalPointToWorldPoint(const sf::Vector2f& p)
		{
			return GetLocalToWorldTransformMatrix().transformPoint(p);
		}

}

