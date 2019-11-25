#include <ML/Audio/OpenAL.hpp>
#include <ML/Core/Debug.hpp>
#include <al/al.h>
#include <al/alc.h>

namespace ml
{
	// Initialization
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool OpenAL::init()
	{
		static bool check = false;
		if (!check)
		{
			check = true;
			ML_AL.m_good = 
				openDevice() &&
				createContext() &&
				makeContextCurrent();
		}
		return ML_AL.m_good;
	}

	void OpenAL::test()
	{
		ALCdevice * device { nullptr };
		if (!(device = alcOpenDevice(NULL)))
		{
			/* error */
		}
	}
	

	// Errors
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	AL::Err OpenAL::getError()
	{
		return static_cast<AL::Err>(alGetError());
	}

	std::ostream & OpenAL::checkError(C_String file, uint32_t line, C_String expr)
	{
		return checkError(std::cout, file, line, expr);
	}

	std::ostream & OpenAL::checkError(std::ostream & out, C_String file, uint32_t line, C_String expr)
	{
		// Get the last error
		const AL::Err code = getError();
		if (code != AL::NoError)
		{
			// Error location
			String fileName { file };
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			// Decode the error
			out << FG::Red		<< "\nAn OpenGL call failed in \'" << file << "\' (" << line << ")"
				<< FG::Yellow	<< "\nCode: "
				<< FG::White	<< "\n\t" << (uint32_t)code
				<< FG::Yellow	<< "\nExpression: "
				<< FG::White	<< "\n\t" << expr
				<< FG::Yellow	<< "\nDescription:"
				<< FG::White	<< "\n\t" << AL::name_of(code)
				<< FG::White	<< "\n\t" << AL::desc_of(code)
				<< FMT()		<< '\n';
		}
		return out;
	}
	

	/* Devices
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
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


	/* Context
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
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


	/* Buffers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t OpenAL::genBuffers(int32_t count)
	{
		static uint32_t temp;
		alCheck(alGenBuffers(count, &temp));
		return temp;
	}

	void OpenAL::deleteBuffers(int32_t count, uint32_t const * buffers)
	{
		alCheck(alDeleteBuffers(count, buffers));
	}

	bool OpenAL::isBuffer(uint32_t id)
	{
		static bool temp;
		alCheck(temp = alIsBuffer(id));
		return temp;
	}

	void OpenAL::bufferData(uint32_t id, int32_t format, void * data, int32_t size, int32_t freq)
	{
		alCheck(alBufferData(id, format, data, size, freq));
	}


	/* Set Buffer Parameters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenAL::bufferf(uint32_t id, int32_t param, float_t value)
	{
		alCheck(alBufferf(id, param, value));
	}

	void OpenAL::buffer3f(uint32_t id, int32_t param, float_t x, float_t y, float_t z)
	{
		alCheck(alBuffer3f(id, param, x, y, z));
	}

	void OpenAL::bufferfv(uint32_t id, int32_t param, float_t const * values)
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

	void OpenAL::bufferiv(uint32_t id, int32_t param, int32_t const * values)
	{
		alCheck(alBufferiv(id, param, values));
	}


	/* Get Buffer Parameters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	float_t OpenAL::getBufferf(uint32_t id, int32_t param)
	{
		static float_t temp;
		alCheck(alGetBufferf(id, param, &temp));
		return temp;
	}

	float_t * OpenAL::getBuffer3f(uint32_t id, int32_t param)
	{
		static float_t temp[3];
		alCheck(alGetBuffer3f(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float_t * OpenAL::getBufferfv(uint32_t id, int32_t param)
	{
		static float_t * temp;
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


	/* Global Parameters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenAL::dopplerFactor(float_t value)
	{
		alCheck(alDopplerFactor(value));
	}

	void OpenAL::dopplerVelocity(float_t value)
	{
		alCheck(alDopplerVelocity(value));
	}

	void OpenAL::speedOfSound(float_t value)
	{
		alCheck(alSpeedOfSound(value));
	}

	void OpenAL::distanceModel(int32_t value)
	{
		alCheck(alDistanceModel(value));
	}


	/* Sources
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t OpenAL::genSources(int32_t count)
	{
		static uint32_t temp;
		alCheck(alGenSources(count, &temp));
		return temp;
	}

	void OpenAL::deleteSources(int32_t count, uint32_t const * sources)
	{
		alCheck(alDeleteSources(count, sources));
	}

	bool OpenAL::isSource(uint32_t id)
	{
		static bool temp;
		alCheck(temp = alIsSource(id));
		return temp;
	}


	/* Set Source Parameters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenAL::sourcef(uint32_t id, int32_t param, float_t value)
	{
		alCheck(alSourcef(id, param, value));
	}

	void OpenAL::source3f(uint32_t id, int32_t param, float_t x, float_t y, float_t z)
	{
		alCheck(alSource3f(id, param, x, y, z));
	}

	void OpenAL::sourcefv(uint32_t id, int32_t param, float_t const * values)
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

	void OpenAL::sourceiv(uint32_t id, int32_t param, int32_t const * values)
	{
		alCheck(alSourceiv(id, param, values));
	}


	/* Get Source Parameters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	float_t OpenAL::getSourcef(uint32_t id, int32_t param)
	{
		static float_t temp;
		alCheck(alGetSourcef(id, param, &temp));
		return temp;
	}

	float_t * OpenAL::getSource3f(uint32_t id, int32_t param)
	{
		static float_t temp[3];
		alCheck(alGetSource3f(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float_t * OpenAL::getSourcefv(uint32_t id, int32_t param)
	{
		static float_t * temp;
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


	/* Source Based Playback calls
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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


	/* Source vector based playback calls
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenAL::sourcePlay(int32_t ns, uint32_t const * sids)
	{
		alCheck(alSourcePlayv(ns, sids));
	}

	void OpenAL::sourceStop(int32_t ns, uint32_t const * sids)
	{
		alCheck(alSourceStopv(ns, sids));
	}

	void OpenAL::sourceRewind(int32_t ns, uint32_t const * sids)
	{
		alCheck(alSourceRewindv(ns, sids));
	}

	void OpenAL::sourcePause(int32_t ns, uint32_t const * sids)
	{
		alCheck(alSourcePausev(ns, sids));
	}


	/* Set Listener Parameters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OpenAL::listenerf(int32_t param, float_t value)
	{
		alCheck(alListenerf(param, value));
	}

	void OpenAL::listener3f(int32_t param, float_t x, float_t y, float_t z)
	{
		alCheck(alListener3f(param, x, y, z));
	}

	void OpenAL::listenerfv(int32_t param, float_t const * values)
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

	void OpenAL::listeneriv(int32_t param, int32_t const * values)
	{
		alCheck(alListeneriv(param, values));
	}


	/* Get Listener Parameters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	float_t OpenAL::getListenerf(int32_t param)
	{
		static float_t temp;
		alCheck(alGetListenerf(param, &temp));
		return temp;
	}

	float_t * OpenAL::getListener3f(int32_t param)
	{
		static float_t temp[3];
		alCheck(alGetListener3f(param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float_t * OpenAL::getListenerfv(int32_t param)
	{
		static float_t * temp;
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}