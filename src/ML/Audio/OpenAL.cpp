#include <ML/Audio/OpenAL.hpp>
#include <ML/Core/Debug.hpp>
#include <AL/al.h>
#include <AL/alc.h>

namespace ml
{
	/* Initialization */

	bool OpenAL::init()
	{
		static bool check = false;
		if (!check && !good())
		{
			check = true;
			openDevice();
			createContext();
			makeContextCurrent();
		}
		return good();
	}
	
	bool OpenAL::good()
	{
		return (ML_AL.m_device && ML_AL.m_context);
	}
	

	/* Errors */
	
	AL::Err OpenAL::getError()
	{
		return static_cast<AL::Err>(alGetError());
	}

	void OpenAL::checkError(CString file, uint32_t line, CString expr)
	{
		// Get the last error
		const AL::Err code = getError();
		if (code != AL::NoError)
		{
			// Error location
			String file(file);
			file = file.substr(file.find_last_of("\\/") + 1);

			// Decode the error
			cout
				<< FMT()		<< ml::endl << FG::Red << "An OpenAL call failed in " << file << "(" << line << ")"
				<< FG::Yellow	<< ml::endl << "Code: "
				<< FG::White	<< ml::endl << "\t" << code
				<< FG::Yellow	<< ml::endl << "Expression: "
				<< FG::White	<< ml::endl << "\t" << expr
				<< FG::Yellow	<< ml::endl << "Description:"
				<< FG::White	<< ml::endl << "\t" << AL::nameOf(code)
				<< FG::White	<< ml::endl << "\t" << AL::descOf(code)
				<< FMT()		<< endl;
		}
	}
	

	/* Devices */
	
	bool OpenAL::openDevice()
	{
		static bool check = false;
		if (!check && !ML_AL.m_device)
		{
			alCheck(check = (ML_AL.m_device = static_cast<ALCdevice *>(alcOpenDevice(NULL))));
		}
		return check;
	}
	
	bool OpenAL::closeDevice()
	{
		static bool check = false;
		if (!check && ML_AL.m_device)
		{
			alCheck(check = alcCloseDevice(static_cast<ALCdevice *>(ML_AL.m_device)));
		}
		return check;
	}


	/* ContextSettings */
	
	bool OpenAL::createContext()
	{
		static bool check = false;
		if (!check && ML_AL.m_device && !ML_AL.m_context)
		{
			alCheck(check = (ML_AL.m_context = static_cast<ALCcontext *>(alcCreateContext(static_cast<ALCdevice *>(ML_AL.m_device), NULL))));
		}
		return check;
	}
	
	bool OpenAL::makeContextCurrent()
	{
		static bool check = false;
		if (!check && ML_AL.m_context)
		{
			alCheck(check = alcMakeContextCurrent(static_cast<ALCcontext *>(ML_AL.m_context)));
		}
		return check;
	}


	/* Buffers */

	uint32_t OpenAL::genBuffers(int32_t count)
	{
		static uint32_t temp;
		alCheck(alGenBuffers(count, &temp));
		return temp;
	}

	void OpenAL::deleteBuffers(int32_t count, const uint32_t * buffers)
	{
		alCheck(alDeleteBuffers(count, buffers));
	}

	bool OpenAL::isBuffer(uint32_t id)
	{
		static bool temp;
		alCheck(temp = alIsBuffer(id));
		return temp;
	}

	void OpenAL::bufferData(uint32_t id, int32_t format, const void * data, int32_t size, int32_t freq)
	{
		alCheck(alBufferData(id, format, data, size, freq));
	}


	/* Set Buffer Parameters */

	void OpenAL::bufferf(uint32_t id, int32_t param, float value)
	{
		alCheck(alBufferf(id, param, value));
	}

	void OpenAL::buffer3f(uint32_t id, int32_t param, float x, float y, float z)
	{
		alCheck(alBuffer3f(id, param, x, y, z));
	}

	void OpenAL::bufferfv(uint32_t id, int32_t param, const float * values)
	{
		alCheck(alBufferfv(id, param, values));
	}

	void OpenAL::bufferi(uint32_t id, int32_t param, int32_t value)
	{
		alCheck(alBufferi(id, param, value));
	}

	void OpenAL::buffer3i(uint32_t id, int32_t param, int32_t x, int32_t y, int32_t z)
	{
		alCheck(alBuffer3i(id, param, x, y, z));
	}

	void OpenAL::bufferiv(uint32_t id, int32_t param, const int32_t * values)
	{
		alCheck(alBufferiv(id, param, values));
	}


	/* Get Buffer Parameters */

	float OpenAL::getBufferf(uint32_t id, int32_t param)
	{
		static float temp;
		alCheck(alGetBufferf(id, param, &temp));
		return temp;
	}

	float * OpenAL::getBuffer3f(uint32_t id, int32_t param)
	{
		static float temp[3];
		alCheck(alGetBuffer3f(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float * OpenAL::getBufferfv(uint32_t id, int32_t param)
	{
		static float * temp;
		alCheck(alGetBufferfv(id, param, temp));
		return temp;
	}

	int32_t OpenAL::getBufferi(uint32_t id, int32_t param)
	{
		static int32_t temp;
		alCheck(alGetBufferi(id, param, &temp));
		return temp;
	}

	int32_t * OpenAL::getBuffer3i(uint32_t id, int32_t param)
	{
		static int32_t temp[3];
		alCheck(alGetBuffer3i(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	int32_t * OpenAL::getBufferiv(uint32_t id, int32_t param)
	{
		static int32_t * temp;
		alCheck(alGetBufferiv(id, param, temp));
		return temp;
	}


	/* Global Parameters */

	void OpenAL::dopplerFactor(float value)
	{
		alCheck(alDopplerFactor(value));
	}

	void OpenAL::dopplerVelocity(float value)
	{
		alCheck(alDopplerVelocity(value));
	}

	void OpenAL::speedOfSound(float value)
	{
		alCheck(alSpeedOfSound(value));
	}

	void OpenAL::distanceModel(int32_t value)
	{
		alCheck(alDistanceModel(value));
	}


	/* Sources */

	uint32_t OpenAL::genSources(int32_t count)
	{
		static uint32_t temp;
		alCheck(alGenSources(count, &temp));
		return temp;
	}

	void OpenAL::deleteSources(int32_t count, const uint32_t * sources)
	{
		alCheck(alDeleteSources(count, sources));
	}

	bool OpenAL::isSource(uint32_t id)
	{
		static bool temp;
		alCheck(temp = alIsSource(id));
		return temp;
	}


	/* Set Source Parameters */

	void OpenAL::sourcef(uint32_t id, int32_t param, float value)
	{
		alCheck(alSourcef(id, param, value));
	}

	void OpenAL::source3f(uint32_t id, int32_t param, float x, float y, float z)
	{
		alCheck(alSource3f(id, param, x, y, z));
	}

	void OpenAL::sourcefv(uint32_t id, int32_t param, const float * values)
	{
		alCheck(alSourcefv(id, param, values));
	}

	void OpenAL::sourcei(uint32_t id, int32_t param, int32_t value)
	{
		alCheck(alSourcei(id, param, value));
	}

	void OpenAL::source3i(uint32_t id, int32_t param, int32_t x, int32_t y, int32_t z)
	{
		alCheck(alSource3i(id, param, x, y, z));
	}

	void OpenAL::sourceiv(uint32_t id, int32_t param, const int32_t * values)
	{
		alCheck(alSourceiv(id, param, values));
	}


	/* Get Source Parameters */

	float OpenAL::getSourcef(uint32_t id, int32_t param)
	{
		static float temp;
		alCheck(alGetSourcef(id, param, &temp));
		return temp;
	}

	float * OpenAL::getSource3f(uint32_t id, int32_t param)
	{
		static float temp[3];
		alCheck(alGetSource3f(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float * OpenAL::getSourcefv(uint32_t id, int32_t param)
	{
		static float * temp;
		alCheck(alGetSourcefv(id, param, temp));
		return temp;
	}

	int32_t OpenAL::getSourcei(uint32_t id, int32_t param)
	{
		static int32_t temp;
		alCheck(alGetSourcei(id, param, &temp));
		return temp;
	}

	int32_t * OpenAL::getSource3i(uint32_t id, int32_t param)
	{
		static int32_t temp[3];
		alCheck(alGetSource3i(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	int32_t * OpenAL::getSourceiv(uint32_t id, int32_t param)
	{
		static int32_t * temp;
		alCheck(alGetSourceiv(id, param, temp));
		return temp;
	}


	/* Source based playback calls */

	void OpenAL::sourcePlay(uint32_t id)
	{
		alCheck(alSourcePlay(id));
	}

	void OpenAL::sourceStop(uint32_t id)
	{
		alCheck(alSourceStop(id));
	}

	void OpenAL::sourceRewind(uint32_t id)
	{
		alCheck(alSourceRewind(id));
	}

	void OpenAL::sourcePause(uint32_t id)
	{
		alCheck(alSourcePause(id));
	}


	/* Source vector based playback calls */

	void OpenAL::sourcePlay(int32_t count, const uint32_t * ids)
	{
		alCheck(alSourcePlayv(count, ids));
	}

	void OpenAL::sourceStop(int32_t count, const uint32_t * ids)
	{
		alCheck(alSourceStopv(count, ids));
	}

	void OpenAL::sourceRewind(int32_t count, const uint32_t * ids)
	{
		alCheck(alSourceRewindv(count, ids));
	}

	void OpenAL::sourcePause(int32_t count, const uint32_t * ids)
	{
		alCheck(alSourcePausev(count, ids));
	}


	/* Set Listener Parameters */

	void OpenAL::listenerf(int32_t param, float value)
	{
		alCheck(alListenerf(param, value));
	}

	void OpenAL::listener3f(int32_t param, float x, float y, float z)
	{
		alCheck(alListener3f(param, x, y, z));
	}

	void OpenAL::listenerfv(int32_t param, const float * values)
	{
		alCheck(alListenerfv(param, values));
	}

	void OpenAL::listeneri(int32_t param, int32_t value)
	{
		alCheck(alListeneri(param, value));
	}

	void OpenAL::listener3i(int32_t param, int32_t x, int32_t y, int32_t z)
	{
		alCheck(alListener3i(param, x, y, z));
	}

	void OpenAL::listeneriv(int32_t param, const int32_t * values)
	{
		alCheck(alListeneriv(param, values));
	}


	/* Get Listener Parameters */

	float OpenAL::getListenerf(int32_t param)
	{
		static float temp;
		alCheck(alGetListenerf(param, &temp));
		return temp;
	}

	float * OpenAL::getListener3f(int32_t param)
	{
		static float temp[3];
		alCheck(alGetListener3f(param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float * OpenAL::getListenerfv(int32_t param)
	{
		static float * temp;
		alCheck(alGetListenerfv(param, temp));
		return temp;
	}

	int32_t OpenAL::getListeneri(int32_t param)
	{
		static int32_t temp;
		alCheck(alGetListeneri(param, &temp));
		return temp;
	}

	int32_t * OpenAL::getListener3i(int32_t param)
	{
		static int32_t temp[3];
		alCheck(alGetListener3i(param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	int32_t * OpenAL::getListeneriv(int32_t param)
	{
		static int32_t * temp;
		alCheck(alGetListeneriv(param, temp));
		return temp;
	}
}