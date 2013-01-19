#pragma once

#include "ofMain.h"

class ofxSimpleParticleEngine
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
	public:
		virtual void update(Particle& p) = 0;
	};
		
	Particle& emit(const ofVec3f& pos, float vel = 1)
	{
		particle_index++;
		if (particle_index > particles.size()) particle_index = 0;
		
		Particle &p = particles[particle_index];
		p.pos = pos;
		
		p.life = ofMap(ofRandom(2), 0, 1, 1, 2);
		
		p.vel.x = ofRandom(-1, 1);
		p.vel.y = ofRandom(-1, 1);
		p.vel.z = ofRandom(-1, 1);
		p.vel.normalize();
		
		p.vel *= ofRandom(vel);
		
		return p;
	}

	void setup(int num)
	{
		particles.resize(num);
		
		time = 1;
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
		glBegin(GL_POINTS);
		for (int i = 0; i < particles.size(); i++)
		{
			Particle &p = particles[i];
			if (p.dead()) continue;
			
			glVertex3fv(p.pos.getPtr());
		}
		glEnd();
		
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


class Gravity : public ofxSimpleParticleEngine::ForceFactor
{
public:
	void update(ofxSimpleParticleEngine::Particle& p)
	{
		p.force.y -= 0.03;
	}
};

class Noise : public ofxSimpleParticleEngine::ForceFactor
{
public:
	
	ofVec3f offset;
	
	void update(ofxSimpleParticleEngine::Particle& p)
	{
		p.force.x += ofSignedNoise(p.pos.x * 0.01) * 0.1;
		p.force.y += ofSignedNoise(p.pos.y * 0.01) * 0.1;
		p.force.z += ofSignedNoise(p.pos.z * 0.01) * 0.1;
		
		p.force.x += ofSignedNoise(offset.x) * 0.1;
		p.force.y += ofSignedNoise(offset.y) * 0.1;
		p.force.z += ofSignedNoise(offset.z) * 0.1;
	}
};

class Floor : public ofxSimpleParticleEngine::ForceFactor
{
public:
	void update(ofxSimpleParticleEngine::Particle& p)
	{
		if (p.pos.y < 0)
		{
			p.force.y = 0;
			p.vel *= 0.5;
			p.vel.y *= -1;
		}
	}
};