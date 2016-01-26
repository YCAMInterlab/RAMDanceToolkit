// 
// ParticleEngine.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ofMain.h"

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
		friend class ParticleEngine;
		
	public:
		virtual void update(Particle& p) = 0;
	};
	
	
	float particle_velocity;
	float particle_life;
	
	vector<ofVec3f> particle_buffer;
	
	ofShader shader;
	
	Particle& emit(const ofVec3f& pos, float vel = 1)
	{
		particle_index++;
		if (particle_index >= particles.size()) particle_index = 0;
		
		Particle &p = particles[particle_index];
		p.pos = pos;
		
		p.life = particle_life * ofRandom(0.5, 1);
		
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
		
#define _S(src) #src
		
		const char *vs = _S(
			uniform float pixel_per_unit;
			uniform float point_size;
			
			void main()
			{
				gl_FrontColor = gl_Color;
				gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
				
				// calc point size
				gl_PointSize = (pixel_per_unit * point_size) / gl_Position.w;
			}
		);
		
#undef _S
		
		shader.setupShaderFromSource(GL_VERTEX_SHADER, vs);
		shader.linkProgram();
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
		
		particle_buffer.clear();
		for (int i = 0; i < particles.size(); i++)
		{
			Particle &p = particles[i];
			if (p.dead()) continue;
			
			particle_buffer.push_back(p.pos);
		}
	}
	
	float getCurrentFov()
	{
		GLfloat m[16];
		GLint viewport[4];
		
		glGetFloatv(GL_PROJECTION_MATRIX, m);
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		float aspect = (float)viewport[2] / viewport[3];
		
		// m[0] / aspect = cot(fovy / 2)
		
		float a = (m[0] * aspect);
		float fov = (atan(a) - M_PI_2) * -2;
		
		return fov;
	}

	void draw()
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

#ifndef TARGET_WIN32
		glEnable(GL_POINT_SMOOTH);
#endif

		float fov = getCurrentFov();
		float pixel_per_unit = fabs(ofGetViewportHeight() / (2.0f * std::tan(fov * 0.5f)));

		shader.begin();
		shader.setUniform1f("pixel_per_unit", pixel_per_unit);
		shader.setUniform1f("point_size", 1);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3 , GL_FLOAT, sizeof(ofVec3f) , &particle_buffer[0]);
		glDrawArrays(GL_POINTS , 0 , particle_buffer.size());
		shader.end();
		
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