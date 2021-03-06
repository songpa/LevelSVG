/*
 * Copyright (c) 2009-2011 Ricardo Quesada
 * Copyright (c) 2011-2012 Zynga Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


#import <Box2D/Box2D.h>

#pragma mark -
#pragma mark ContactListener

class MyContactListener : public b2ContactListener
{
public:
	virtual ~MyContactListener() {}
	
	/// Called when two fixtures begin to touch.
	virtual void BeginContact(b2Contact* contact);
	
	/// Called when two fixtures cease to touch.
	virtual void EndContact(b2Contact* contact);
	
	/// This is called after a contact is updated. This allows you to inspect a
	/// contact before it goes to the solver. If you are careful, you can modify the
	/// contact manifold (e.g. disable contact).
	/// A copy of the old manifold is provided so that you can detect changes.
	/// Note: this is called only for awake bodies.
	/// Note: this is called even when the number of contact points is zero.
	/// Note: this is not called for sensors.
	/// Note: if you set the number of contact points to zero, you will not
	/// get an EndContact callback. However, you may get a BeginContact callback
	/// the next step.
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	
	/// This lets you inspect a contact after the solver is finished. This is useful
	/// for inspecting impulses.
	/// Note: the contact manifold does not include time of impact impulses, which can be
	/// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
	/// in a separate data structure.
	/// Note: this is only called for contacts that are touching, solid, and awake.
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#pragma mark -
#pragma mark ContactFilter

class MyContactFilter : public b2ContactFilter
{
public:
	virtual ~MyContactFilter() {}
	
	/// Return true if contact calculations should be performed between these two shapes.
	/// @warning for performance reasons this is only called when the AABBs begin to overlap.
	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	
	/// Return true if the given shape should be considered for ray intersection
	virtual bool RayCollide(void* userData, b2Fixture* fixture);
	
};

#pragma mark -
#pragma mark DestructionListener

class MyDestructionListener : public b2DestructionListener
{
public:
	virtual ~MyDestructionListener() {}
	
	/// Called when any joint is about to be destroyed due
	/// to the destruction of one of its attached bodies.
	virtual void SayGoodbye(b2Joint* joint);
	
	/// Called when any fixture is about to be destroyed due
	/// to the destruction of its parent body.
	virtual void SayGoodbye(b2Fixture* fixture);
	
};


#pragma mark -
#pragma mark QueryCallback

//
// This class is based on the Box2dTestBed example.
// A simple QueryCallback that test if point is touching a non-static fixture.
// If so, it returns it.
//
class MyQueryCallback : public b2QueryCallback
{
public:
	MyQueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}
	
	/// Called for each fixture found in the query AABB.
	/// @return false to terminate the query.
	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;
				
				// We are done, terminate the query.
				return false;
			}
		}
		
		// Continue the query.
		return true;
	}
	
	b2Vec2 m_point;
	b2Fixture* m_fixture;
};
