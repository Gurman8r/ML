#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Editor/TerminalGui.hpp>
#include <ML/Editor/BuilderGui.hpp>
#include <ML/Editor/BrowserGui.hpp>
#include <ML/Editor/DockspaceGui.hpp>
#include <ML/Editor/ResourceGui.hpp>
#include <ML/Editor/ProfilerGui.hpp>
#include <ML/Engine/EngineEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public IObject
		, public EventListener
		, public INonCopyable
		, public IDisposable
	{
	public:
		explicit Editor(EventSystem & eventSystem);
		~Editor();

	public:
		bool dispose() override;
		void onEvent(const Event * value) override;

	private:
		void onEnter	(const EnterEvent & ev);
		void onBeginGui	(const BeginGuiEvent & ev);
		void onGui		(const GuiEvent & ev);
		void onEndGui	(const EndGuiEvent & ev);
		void onExit		(const ExitEvent & ev);

	public:
		template <
			class T, class ... Args
		> inline T * create(const String & name, Args && ... args)
		{
			Map<String, EditorGui *> & data = m_gui[typeid(T).hash_code()];
			Map<String, EditorGui *>::iterator it;
			return ((it = data.find(name)) == data.end())
				? static_cast<T *>(
					data.insert({
						name, new T(std::forward<Args>(args)...)
						}).first->second)
				: static_cast<T *>(nullptr);
		}

		template <
			class T
		> inline bool erase(const String & name)
		{
			Map<String, EditorGui *> & data = m_gui[typeid(T).hash_code()];
			Map<String, EditorGui *>::iterator it;
			if ((it = data.find(name)) != data.end())
			{
				delete it->second;
				it->second = nullptr;
				data.erase(it);
				return true;
			}
			return false;
		}

		template <
			class T
		> inline const T * get(const String & name = String()) const
		{
			const Map<String, EditorGui *> & data = m_gui[typeid(T).hash_code()];
			Map<String, EditorGui *>::const_iterator it;
			return ((it = data.find(name)) != data.end())
				? static_cast<const T *>(it->second)
				: static_cast<const T *>(nullptr);
		}

		template <
			class T
		> inline T * get(const String & name = String())
		{
			Map<String, EditorGui *> & data = m_gui[typeid(T).hash_code()];
			Map<String, EditorGui *>::iterator it;
			return ((it = data.find(name)) != data.end())
				? static_cast<T *>(it->second)
				: static_cast<T *>(nullptr);
		}

	private:
		HashMap<size_t, Map<String, EditorGui *>> m_gui;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_