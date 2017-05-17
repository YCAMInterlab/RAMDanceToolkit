// 
// ramUnit.h - RAMDanceToolkit
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

#include "ramControllable.h"
#include "ramGlobal.h"

namespace rdtk{
	class Unit : public Controllable, public GlobalShortcut
	{
	public:
		Unit() : bEnabled(false) {}
		virtual ~Unit() {}
		
		void enable()
		{
			bEnabled = true;
		}
		
		void disable()
		{
			bEnabled = false;
		}
		
		void toggle()
		{
			setEnabled(!isEnabled());
		}
		
		bool isEnabled() const
		{
			return bEnabled;
		}
		
		void setEnabled(bool enabled)
		{
			if (bEnabled == enabled) return;
			
			bEnabled = enabled;
			
			if (enabled)
				onEnabled();
			else
				onDisabled();
		}
		
		virtual void loadPreset(size_t preset_id = 0)
		{
			cout << "[loadPreset] " << getName() << endl;
		}
		
		virtual void loadPresetXML(string filePath)
		{
			cout << "[loadPresetXML] " << getName() << endl;
		}
		
		virtual void onEnabled()
		{
			cout << "[Unit enabled] " << getName() << endl;
		}
		
		virtual void onDisabled()
		{
			cout << "[Unit disabled] " << getName() << endl;
		}
		
	protected:
		
		bool bEnabled;
	};
}

using RAMDEPRECATED(ramUnit) = rdtk::Unit;
