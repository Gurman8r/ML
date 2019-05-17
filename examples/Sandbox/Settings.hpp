#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

#include <INIReader.h>
#include <ML/Core/FileSystem.hpp>
#include <ML/Window/Window.hpp>

// Singleton to store program properties/settings for Sandbox (INIReader)
#define SETTINGS DEMO::Settings::getInstance()

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Settings final
		: public ml::ITrackable
		, public ml::IReadable
		, public ml::ISingleton<Settings>
	{	
		friend class ml::ISingleton<Settings>;

	public:
		ml::String	configINI;		// path to config INI

	public:
		// [General]
		/* * * * * * * * * * * * * * * * * * * * */
		bool		escapeIsExit;	// Escape key is Exit?
		ml::String	manifest;		// Name of manifest file
		bool		redirStdOut;	// Redirect Std Out?

		// [ImGui]
		/* * * * * * * * * * * * * * * * * * * * */
		ml::String	imguiINI;		// Name of imgui INI
		ml::String	imguiFontFile;	// Name of imgui TTF
		float		imguiFontSize;	// Size of imgui TTF

		// [Script]
		/* * * * * * * * * * * * * * * * * * * * */
		ml::String	bootScript;		// Script to run on start
		bool		flagShowToks;	// Show Script Tokens
		bool		flagShowTree;	// Show Script Syntax Tree
		bool		flagShowItoP;	// Show Script Infix to Postfix

		// [Window]
		/* * * * * * * * * * * * * * * * * * * * */
		ml::String	title;			// Window Title
		uint32_t	width;			// Window Width
		uint32_t	height;			// Window Height
		uint32_t	bitsPerPixel;	// Bits Per Pixel
		uint32_t	majorVersion;	// Major Version
		uint32_t	minorVersion;	// Minor Version
		uint32_t	depthBits;		// Depth Bits
		uint32_t	stencilBits;	// Stencil Bits
		uint32_t	profile;		// Profile
		bool		multisample;	// Multisample
		bool		srgbCapable;	// sRGB Capable


		// [Graphics]
		/* * * * * * * * * * * * * * * * * * * * */
		float		fieldOfView;	// Field of View
		float		perspNear;		// Persp Near Clipping Range 
		float		perspFar;		// Persp Far  Clipping Range 
		float		orthoNear;		// Ortho Near Clipping Range 
		float		orthoFar;		// Ortho Far  Clipping Range

		// [Network]
		/* * * * * * * * * * * * * * * * * * * * */
		bool		isServer;		// Is Server?
		bool		isClient;		// Is Client?


	public:
		inline bool loadFromFile(const ml::String & value) override
		{
			// Read INI
			INIReader ini(configINI = value);
			bool good;
			if (good = (ini.ParseError() == EXIT_SUCCESS))
			{
				// [General]
				/* * * * * * * * * * * * * * * * * * * * */
				escapeIsExit	= ini.GetBoolean("General", "escapeIsExit", true);
				manifest		= ini.Get("General", "manifest", "manifest.txt");
				redirStdOut		= ini.GetBoolean("General", "redirStdOut", false);
				
				// [ImGui]
				/* * * * * * * * * * * * * * * * * * * * */
				imguiINI		= ini.Get("ImGui", "imguiINI", "");
				imguiFontFile	= ini.Get("ImGui", "imguiFontFile", "");
				imguiFontSize	= (float)ini.GetReal("ImGui", "imguiFontSize", 12.0f);

				// [Script]
				/* * * * * * * * * * * * * * * * * * * * */
				bootScript		= ini.Get("Script", "bootScript", "/scripts/hello.meme");
				flagShowToks	= ini.GetBoolean("Script", "flagShowToks", false);
				flagShowTree	= ini.GetBoolean("Script", "flagShowTree", false);
				flagShowItoP	= ini.GetBoolean("Script", "flagShowItoP", false);

				// [Window]
				/* * * * * * * * * * * * * * * * * * * * */
				title			= ini.Get("Window", "title", "Window");
				width			= ini.GetInteger("Window", "width", 640);
				height			= ini.GetInteger("Window", "height", 480);
				bitsPerPixel	= ini.GetInteger("Window", "bitsPerPixel", 32);
				majorVersion	= ini.GetInteger("Window", "majorVersion", 3);
				minorVersion	= ini.GetInteger("Window", "minorVersion", 3);
				depthBits		= ini.GetInteger("Window", "depthBits", 24);
				stencilBits		= ini.GetInteger("Window", "stencilBits", 8);
				profile			= ini.GetInteger("Window", "profile", ml::ContextSettings::Compat);
				multisample		= ini.GetBoolean("Window", "multisample", false);
				srgbCapable		= ini.GetBoolean("Window", "srgbCapable", false);

				// [Graphics]	
				/* * * * * * * * * * * * * * * * * * * * */
				fieldOfView		= (float)ini.GetReal("Graphics", "fieldOfView", 45.0);
				perspNear		= (float)ini.GetReal("Graphics", "perspNear", 0.1);
				perspFar		= (float)ini.GetReal("Graphics", "perspFar", 1000.0);
				orthoNear		= (float)ini.GetReal("Graphics", "orthoNear", -1.0);
				orthoFar		= (float)ini.GetReal("Graphics", "orthoFar", +1.0);

				// [Network]
				/* * * * * * * * * * * * * * * * * * * * */
				isServer		= ini.GetBoolean("Network", "isServer", false);
				isClient		= ini.GetBoolean("Network", "isClient", false);
			}
			return good;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SETTINGS_HPP_