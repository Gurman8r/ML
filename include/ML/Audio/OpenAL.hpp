#ifndef _ML_OPEN_AL_HPP_
#define _ML_OPEN_AL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Audio/Export.hpp>
#include <ML/Audio/AL.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/Singleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_AL ::ml::OpenAL::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

# if not ML_DEBUG
#	define alCheck(expr) (expr)
# else
#	define alCheck(expr) do { expr; ML_AL.checkError(__FILE__, __LINE__, #expr); } while (false)
# endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API OpenAL final
		: public Singleton<OpenAL>
	{
		friend struct Singleton<OpenAL>;

		bool	m_good;
		void *	m_device;
		void *	m_context;

	public:

		// Initialization
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool init();
		static void test();

		// Errors
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto	getError() -> AL::Err;
		static auto	checkError(C_String file, uint32_t line, C_String expr) -> std::ostream &;
		static auto	checkError(std::ostream & out, C_String file, uint32_t line, C_String expr) -> std::ostream &;

		// Devices
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	openDevice();
		static bool	closeDevice();

		// Context
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool	createContext();
		static bool	makeContextCurrent();

		// Buffers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto	genBuffers(int32_t count) -> uint32_t;
		static void	deleteBuffers(int32_t count, const uint32_t * buffers);
		static bool	isBuffer(uint32_t id);
		static void	bufferData(uint32_t id, int32_t format, const void * data, int32_t size, int32_t freq);
		
		// Set Buffer Parameters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	bufferf(uint32_t id, int32_t param, float_t value);
		static void	buffer3f(uint32_t id, int32_t param, float_t x, float_t y, float_t z);
		static void	bufferfv(uint32_t id, int32_t param, const float_t * values);
		static void	bufferi(uint32_t id, int32_t param, int32_t value);
		static void	buffer3i(uint32_t id, int32_t param, int32_t x, int32_t y, int32_t z);
		static void	bufferiv(uint32_t id, int32_t param, const int32_t * values);

		// Get Buffer Parameters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto getBufferf(uint32_t id, int32_t param) -> float_t;
		static auto getBuffer3f(uint32_t id, int32_t param)	-> float_t *;
		static auto getBufferfv(uint32_t id, int32_t param)	-> float_t *;
		static auto getBufferi(uint32_t id, int32_t param) -> int32_t;
		static auto getBuffer3i(uint32_t id, int32_t param) -> int32_t *;
		static auto getBufferiv(uint32_t id, int32_t param)	-> int32_t *;

		// Global Parameters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	dopplerFactor(float_t value);
		static void	dopplerVelocity(float_t value);
		static void	speedOfSound(float_t value);
		static void	distanceModel(int32_t value);

		// Sources
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto	genSources(int32_t count) -> uint32_t;
		static void	deleteSources(int32_t count, const uint32_t * sources);
		static bool	isSource(uint32_t id);

		// Set Source Parameters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	sourcef(uint32_t id, int32_t param, float_t value);
		static void	source3f(uint32_t id, int32_t param, float_t x, float_t y, float_t z);
		static void	sourcefv(uint32_t id, int32_t param, const float_t * values);
		static void	sourcei(uint32_t id, int32_t param, int32_t value);
		static void	source3i(uint32_t id, int32_t param, int32_t x, int32_t y, int32_t z);
		static void	sourceiv(uint32_t id, int32_t param, const int32_t * values);

		// Get Source Parameters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto getSourcef(uint32_t id, int32_t param) -> float_t;
		static auto getSource3f(uint32_t id, int32_t param)	-> float_t *;
		static auto getSourcefv(uint32_t id, int32_t param)	-> float_t *;
		static auto getSourcei(uint32_t id, int32_t param) -> int32_t;
		static auto getSource3i(uint32_t id, int32_t param)	-> int32_t *;
		static auto getSourceiv(uint32_t id, int32_t param)	-> int32_t *;

		// Source Based Playback calls
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	sourcePlay(uint32_t id);
		static void	sourceStop(uint32_t id);
		static void	sourceRewind(uint32_t id);
		static void	sourcePause(uint32_t id);

		// Source Based Playback calls
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	sourcePlay(int32_t ns, const uint32_t * sids);
		static void	sourceStop(int32_t ns, const uint32_t * sids);
		static void	sourceRewind(int32_t ns, const uint32_t * sids);
		static void	sourcePause(int32_t ns, const uint32_t * sids);

		// Set Listener Parameters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static void	listenerf(int32_t param, float_t value);
		static void	listener3f(int32_t param, float_t x, float_t y, float_t z);
		static void	listenerfv(int32_t param, const float_t * values);
		static void	listeneri(int32_t param, int32_t value);
		static void	listener3i(int32_t param, int32_t x, int32_t y, int32_t z);
		static void	listeneriv(int32_t param, const int32_t * values);

		// Get Source Parameters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static auto getListenerf(int32_t param)	-> float_t;
		static auto getListener3f(int32_t param) -> float_t *;
		static auto getListenerfv(int32_t param) -> float_t *;
		static auto getListeneri(int32_t param) -> int32_t;
		static auto getListener3i(int32_t param) -> int32_t *;
		static auto getListeneriv(int32_t param) -> int32_t *;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_OPEN_AL_HPP_