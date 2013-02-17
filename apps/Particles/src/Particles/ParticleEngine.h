#pragma once

class ParticleEngine
{
	
public:
	
	class Particle
	{
	public:
		ofVec3f pos;
		ofVec3f vel;
		ofVec3f force;
		
		float life;
		
		Particle() : vel(0, 0, 0), force(0, 0, 0), life(0) {}
		
		bool alive() const { return life > 0; }
		bool dead() const { return life <= 0; }
	};
	
	class ForceFactor
	{
		friend ParticleEngine;
		
	public:
		virtual void update(Particle& p) = 0;
	};
	
	
	float particle_velocity;
	float particle_life;
	
	Particle& emit(const ofVec3f& pos, float vel = 1)
	{
		particle_index++;
		if (particle_index > particles.size()) particle_index = 0;
		
		Particle &p = particles[particle_index];
		p.pos = pos;
		
		p.life = particle_life;
		
		p.vel.x = ofRandom(-1, 1);
		p.vel.y = ofRandom(-1, 1);
		p.vel.z = ofRandom(-1, 1);
		p.vel.normalize();
		
		p.vel *= ofRandom(particle_velocity);
		
		return p;
	}
	
	void setup(int num)
	{
		particles.resize(num);
		
		time = 1;
		particle_velocity = 2;
		particle_life = 2;
	}
	
	void update()
	{
		const float t = time;
		const float frame_time = ofGetLastFrameTime() * t;
		
		// pass 1
		for (int i = 0; i < particles.size(); i++)
		{
			Particle &p = particles[i];
			if (p.dead()) continue;
			
			// update lifetime
			p.life -= frame_time;
			
			// clear force
			p.force.set(0, 0, 0);
		}
		
		// pass 2
		for (int i = 0; i < force_factors.size(); i++)
		{
			ForceFactor *ff = force_factors[i];
			
			for (int n = 0; n < particles.size(); n++)
			{
				Particle &p = particles[n];
				if (p.dead()) continue;
				
				// update force
				ff->update(p);
			}
		}
		
		// pass 3
		for (int i = 0; i < particles.size(); i++)
		{
			Particle &p = particles[i];
			if (p.dead()) continue;
			
			// update position
			p.vel += p.force * t;
			p.pos += p.vel * t;
		}
	}
	
	void draw()
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		
		glEnable(GL_POINT_SMOOTH);
		glDisable(GL_DEPTH_TEST);
		glPointSize(500);
		
		static GLfloat distance[] = { 0.0, 0.0, 1.0 };
		glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distance);
		
		glBegin(GL_POINTS);
		for (int i = 0; i < particles.size(); i++)
		{
			Particle &p = particles[i];
			if (p.dead()) continue;
			
			glVertex3fv(p.pos.getPtr());
		}
		glEnd();
		ofPopStyle();
		glPopAttrib();
	}
	
	void addForceFactor(ForceFactor *o)
	{
		force_factors.push_back(o);
	}
	
protected:
	
	float time;
	
	int particle_index;
	vector<Particle> particles;
	vector<ForceFactor*> force_factors;
	
};

class Gravity : public ParticleEngine::ForceFactor
{
public:
	Gravity() : force(0.0001) {}
	
	float force;
	
	void update(ParticleEngine::Particle& p)
	{
		p.force.y -= force;
	}
};

class Floor : public ParticleEngine::ForceFactor
{
public:
	Floor() {}
	
	void update(ParticleEngine::Particle& p)
	{
		if (p.pos.y < 0)
		{
			p.force.y = 0;
			p.vel *= 0.5;
			p.vel.y *= -1;
		}
	}
};