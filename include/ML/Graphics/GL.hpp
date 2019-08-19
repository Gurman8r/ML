#ifndef _ML_GL_HPP_
#define _ML_GL_HPP_

#include <ML/Core/StandardLib.hpp>

// OpenGL Enums
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	namespace GL
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Flag : uint32_t
		{
			CullFace		= 0x0B44,		// GL_CULL_FACE
			DepthTest		= 0x0B71,		// GL_DEPTH_TEST
			AlphaTest		= 0x0BC0,		// GL_ALPHA_TEST
			Blend			= 0x0BE2,		// GL_BLEND
			Multisample		= 0x809D,		// GL_MULTISAMPLE
			FramebufferSRGB	= 0x8DB9,		// GL_FRAMEBUFFER_SRGB
			ScissorTest		= 0x0C11,		// GL_SCISSOR_TEST
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Sampler : uint32_t
		{
			Texture2D = 0x0DE1,				// GL_TEXTURE_2D
			Texture3D = 0x806F,				// GL_TEXTURE_3D
			TextureCubeMap = 0x8513,		// GL_TEXTURE_CUBE_MAP
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Target : uint32_t
		{
			ArrayBuffer	= 0x8892,			// GL_ARRAY_BUFFER
			ElementArrayBuffer,				// GL_ELEMENT_ARRAY_BUFFER
			ArrayBufferBinding,				// GL_ARRAY_BUFFER_BINDING
			Framebuffer = 0x8D40,			// GL_FRAMEBUFFER
			Renderbuffer,					// GL_RENDERBUFFER
			ProgramObject = 0x8B40,			// GL_PROGRAM_OBJECT_ARB
			FramebufferRead = 0x8CA8,		// GL_READ_FRAMEBUFFER
			FramebufferDraw = 0x8CA9,		// GL_DRAW_FRAMEBUFFER
			ReadFramebufferBinding = 0x8CAA,// GL_READ_FRAMEBUFFER_BINDING
			DrawFramebufferBinding = 0x8CA6,// GL_DRAW_FRAMEBUFFER_BINDING
			CurrentProgram = 0x8B8D,		// GL_CURRENT_PROGRAM
			VertexArrayBinding = 0x85B5,	// GL_VERTEX_ARRAY_BINDING
			TextureBinding2D = 0x8069,		// GL_TEXTURE_BINDING_2D
			SamplerBinding = 0x8919,		// GL_SAMPLER_BINDING
			ActiveTexture = 0x84E0,			// GL_ACTIVE_TEXTURE
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Usage : uint32_t
		{
			StreamDraw = 0x88E0,			// GL_STREAM_DRAW
			StaticDraw = 0x88E4,			// GL_STATIC_DRAW
			DynamicDraw	= 0x88E8,			// GL_DYNAMIC_DRAW
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Err : uint32_t
		{
			NoError = 0,					// GL_NO_ERROR
			InvalidEnum = 0x0500,			// GL_INVALID_ENUM
			InvalidValue,					// GL_INVALID_VALUE
			InvalidOperation,				// GL_INVALID_OPERATION
			StackOverflow,					// GL_STACK_OVERFLOW
			StackUnderflow,					// GL_STACK_UNDERFLOW
			OutOfMemory,					// GL_OUT_OF_MEMORY
			InvalidFramebufferOperation,	// GL_INVALID_FRAMEBUFFER_OPERATION
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum StringID : uint32_t
		{
			Vendor = 0x1F00,				// GL_VENDOR
			Renderer,						// GL_RENDERER
			Version,						// GL_VERSION
			Extensions,						// GL_EXTENSIONS
			ShadingLanguageVersion = 0x8B8C,// GL_SHADING_LANGUAGE_VERSION			
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		enum IntID : uint32_t
		{
			MajorVersion = 0x821B,			// GL_MAJOR_VERSION
			MinorVersion,					// GL_MINOR_VERSION
			NumExtensions,					// GL_NUM_EXTENSIONS
			ContextFlags = 0x821E,			// GL_CONTEXT_FLAGS
			ContextProfileMask = 0x9126,	// GL_CONTEXT_PROFILE_MASK
			MaxTextureSize = 0x0D33,		// GL_MAX_TEXTURE_SIZE
			MaxCombTexImgUnits = 0x8B4D,	// GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
			InfoLogLength = 0x8B84,			// GL_INFO_LOG_LENGTH
			PolygonMode = 0x0B40,			// GL_POLYGON_MODE
			Viewport = 0x0BA2,				// GL_VIEWPORT
			ScissorBox = 0x0C10,			// GL_SCISSOR_BOX
			BlendEquationRGB = 0x8009,		// GL_BLEND_EQUATION_RGB
			BlendEquationAlpha = 0x883D,	// GL_BLEND_EQUATION_ALPHA
			BlendDestRGB = 0x80C8,			// GL_BLEND_DST_RGB
			BlendSourceRGB,					// GL_BLEND_SRC_RGB
			BlendDestAlpha = 0x80CA,		// GL_BLEND_DST_ALPHA
			BlendSourceAlpha,				// GL_BLEND_SRC_ALPHA
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Clip : uint32_t
		{
			LowerLeft = 0x8CA1,				// GL_LOWER_LEFT
			UpperLeft,						// GL_UPPER_LEFT
			ClipOrigin = 0x935C,			// GL_CLIP_ORIGIN
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Status : uint32_t
		{
			ObjectDeleteStatus = 0x8B80,	// GL_OBJECT_DELETE_STATUS_ARB
			ObjectCompileStatus,			// GL_OBJECT_COMPILE_STATUS_ARB
			ObjectLinkStatus,				// GL_OBJECT_LINK_STATUS_ARB
			FramebufferComplete = 0x8CD5,	// GL_FRAMEBUFFER_COMPLETE
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum ShaderType : uint32_t
		{
			FragmentShader = 0x8B30,		// GL_FRAGMENT_SHADER
			VertexShader,					// GL_VERTEX_SHADER
			GeometryShader = 0x8DD9,		// GL_GEOMETRY_SHADER
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Mode : uint32_t
		{
			Points = 0x000,					// GL_POINTS
			Lines,							// GL_LINES
			LineLoop,						// GL_LINE_LOOP
			LineStrip,						// GL_LINE_STRIP
			Triangles,						// GL_TRIANGLES
			TriangleStrip,					// GL_TRIANGLE_STRIP
			TriangleFan,					// GL_TRIANGLE_FAN
			Fill = 0x1B02,					// GL_FILL
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Equation : uint32_t
		{
			Multiply = 0x0103,				// GL_MULT
			Add	= 0x0104,					// GL_ADD
			Subtract = 0x84E7,				// GL_SUBTRACT
			FuncAdd	= 0x8006,				// GL_FUNC_ADD
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Predicate : uint32_t
		{
			Never = 0x200,					// GL_NEVER
			Less,							// GL_LESS
			Equal,							// GL_EQUAL
			LEqual,							// GL_LEQUAL
			Greater,						// GL_GREATER
			NotEqual,						// GL_NOTEQUAL
			GEqual,							// GL_GEQUAL
			Always,							// GL_ALWAYS
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Factor : uint32_t
		{
			SrcColor = 0x0300,				// GL_SRC_COLOR
			OneMinusSrcColor,				// GL_ONE_MINUS_SRC_COLOR
			SrcAlpha,						// GL_SRC_ALPHA
			OneMinusSrcAlpha,				// GL_ONE_MINUS_SRC_ALPHA
			DstAlpha,						// GL_DST_ALPHA
			OneMinusDstAlpha,				// GL_ONE_MINUS_DST_ALPHA
			DstColor,						// GL_DST_COLOR
			OneMinusDstColor,				// GL_ONE_MINUS_DST_COLOR
			SrcAlphaSaturate,				// GL_SRC_ALPHA_SATURATE
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Face : uint32_t
		{
			FrontLeft = 0x0400,				// GL_FRONT_LEFT
			FrontRight,						// GL_FRONT_RIGHT
			BackLeft,						// GL_BACK_LEFT
			BackRight,						// GL_BACK_RIGHT
			Front,							// GL_FRONT
			Back,							// GL_BACK
			Left,							// GL_LEFT
			Right,							// GL_RIGHT
			FrontAndBack,					// GL_FRONT_AND_BACK
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Type : uint32_t
		{
			Byte = 0x1400,					// GL_BYTE
			UnsignedByte,					// GL_UNSIGNED_BYTE
			Short,							// GL_SHORT
			UnsignedShort,					// GL_UNSIGNED_SHORT
			Int,							// GL_INT
			UnsignedInt,					// GL_UNSIGNED_INT
			Float,							// GL_FLOAT
			HalfFloat,						// GL_HALF_FLOAT
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Format : uint32_t
		{
			Red = 0x1903,					// GL_RED
			Green,							// GL_GREEN
			Blue,							// GL_BLUE
			Alpha,							// GL_ALPHA
			RGB,							// GL_RGB
			RGBA,							// GL_RGBA
			Luminance,						// GL_LUMINANCE
			Luminance_Alpha,				// GL_LUMINANCE_ALPHA

			SRGB = 0x8C40,					// GL_SRGB
			SRGB8,							// GL_SRGB8
			SRGB_Alpha,						// GL_SRGB_ALPHA
			SRGB8_Alpha8,					// GL_SRGB8_ALPHA8
			SLuminance_Alpha,				// GL_SLUMINANCE_ALPHA
			SLuminance8_Alpha8,				// GL_SLUMINANCE8_ALPHA8
			SLuminance,						// GL_SLUMINANCE
			SLuminance8,					// GL_SLUMINANCE8

			Depth24_Stencil8 = 0x88F0,		// GL_DEPTH24_STENCIL8
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum FrameAttachment : uint32_t
		{
			ColorEncoding= 0x8210,			// GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING
			ComponentType,					// GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE
			RedSize,						// GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE
			GreenSize,						// GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE
			BlueSize,						// GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE
			AlphaSize,						// GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE
			DepthSize,						// GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE
			StencilSize,					// GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE
			FramebufferDefault,				// GL_FRAMEBUFFER_DEFAULT
			FramebufferUndefined,			// GL_FRAMEBUFFER_UNDEFINED
			DepthStencil,					// GL_DEPTH_STENCIL_ATTACHMENT
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum TexParam : uint32_t
		{
			Nearest = 0x2600,				// GL_NEAREST
			Linear,							// GL_LINEAR
			NearestMipmapNearest = 0x2700,	// GL_NEAREST_MIPMAP_NEAREST
			LinearMipmapNearest,			// GL_LINEAR_MIPMAP_NEAREST
			NearestMipmapLinear,			// GL_NEAREST_MIPMAP_LINEAR
			LinearMipmapLinear,				// GL_LINEAR_MIPMAP_LINEAR
			TexMagFilter = 0x2800,			// GL_TEXTURE_MAG_FILTER
			TexMinFilter,					// GL_TEXTURE_MIN_FILTER
			TexWrapS,						// GL_TEXTURE_WRAP_S
			TexWrapT,						// GL_TEXTURE_WRAP_T
			TexWrapR = 0x8072,				// GL_TEXTURE_WRAP_R
			Clamp = 0x2900,					// GL_CLAMP
			Repeat,							// GL_REPEAT
			ClampToEdge = 0x812F,			// GL_CLAMP_TO_EDGE
			MinLOD,							// GL_TEXTURE_MIN_LOD
			MaxLOD,							// GL_TEXTURE_MAX_LOD
			BaseLevel,						// GL_TEXTURE_BASE_LEVEL
			MaxLevel,						// GL_TEXTURE_MAX_LEVEL

			CubeMap_Positive_X = 0x8515,	// GL_TEXTURE_CUBE_MAP_POSITIVE_X
			CubeMap_Negative_X,				// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
			CubeMap_Positive_Y,				// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
			CubeMap_Negative_Y,				// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
			CubeMap_Positive_Z,				// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
			CubeMap_Negative_Z,				// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Pack : uint32_t
		{
			UnpackSwapBytes = 0x0CF0,		// GL_UNPACK_SWAP_BYTES
			UnpackLsbFirst,					// GL_UNPACK_LSB_FIRST
			UnpackRowLength,				// GL_UNPACK_ROW_LENGTH
			UnpackSkipRows,					// GL_UNPACK_SKIP_ROWS
			UnpackSkipPixels,				// GL_UNPACK_SKIP_PIXELS
			UnpackAlignment,				// GL_UNPACK_ALIGNMENT
		
			PackSwapBytes = 0x0D00,			// GL_PACK_SWAP_BYTES
			PackLsbFirst,					// GL_PACK_LSB_FIRST
			PackRowLength,					// GL_PACK_ROW_LENGTH
			PackSkipRows,					// GL_PACK_SKIP_ROWS
			PackSkipPixels,					// GL_PACK_SKIP_PIXELS
			PackAlignment,					// GL_PACK_ALIGNMENT
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum ColorAttachment : uint32_t
		{
			ColorAttachment0 = 0x8CE0,		// GL_COLOR_ATTACHMENT0
			ColorAttachment1,				// GL_COLOR_ATTACHMENT1
			ColorAttachment2,				// GL_COLOR_ATTACHMENT2
			ColorAttachment3,				// GL_COLOR_ATTACHMENT3
			ColorAttachment4,				// GL_COLOR_ATTACHMENT4
			ColorAttachment5,				// GL_COLOR_ATTACHMENT5
			ColorAttachment6,				// GL_COLOR_ATTACHMENT6
			ColorAttachment7,				// GL_COLOR_ATTACHMENT7
			ColorAttachment8,				// GL_COLOR_ATTACHMENT8
			ColorAttachment9,				// GL_COLOR_ATTACHMENT9
			ColorAttachment10,				// GL_COLOR_ATTACHMENT10
			ColorAttachment11,				// GL_COLOR_ATTACHMENT11
			ColorAttachment12,				// GL_COLOR_ATTACHMENT12
			ColorAttachment13,				// GL_COLOR_ATTACHMENT13
			ColorAttachment14,				// GL_COLOR_ATTACHMENT14
			ColorAttachment15,				// GL_COLOR_ATTACHMENT15
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum TexID : uint32_t
		{
			Texture0 = 0x84C0,				// GL_TEXTURE0
			Texture1,						// GL_TEXTURE1
			Texture2,						// GL_TEXTURE2
			Texture3,						// GL_TEXTURE3
			Texture4,						// GL_TEXTURE4
			Texture5,						// GL_TEXTURE5
			Texture6,						// GL_TEXTURE6
			Texture7,						// GL_TEXTURE7
			Texture8,						// GL_TEXTURE8
			Texture9,						// GL_TEXTURE9
			Texture10,						// GL_TEXTURE10
			Texture11,						// GL_TEXTURE11
			Texture12,						// GL_TEXTURE12
			Texture13,						// GL_TEXTURE13
			Texture14,						// GL_TEXTURE14
			Texture15,						// GL_TEXTURE15
			Texture16,						// GL_TEXTURE16
			Texture17,						// GL_TEXTURE17
			Texture18,						// GL_TEXTURE18
			Texture19,						// GL_TEXTURE19
			Texture20,						// GL_TEXTURE20
			Texture21,						// GL_TEXTURE21
			Texture22,						// GL_TEXTURE22
			Texture23,						// GL_TEXTURE23
			Texture24,						// GL_TEXTURE24
			Texture25,						// GL_TEXTURE25
			Texture26,						// GL_TEXTURE26
			Texture27,						// GL_TEXTURE27
			Texture28,						// GL_TEXTURE28
			Texture29,						// GL_TEXTURE29
			Texture30,						// GL_TEXTURE30
			Texture31,						// GL_TEXTURE31
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum DrawBuffer : uint32_t
		{
			DrawBuffer0 = 0x8CE0,			// GL_DRAW_BUFFER0
			DrawBuffer1,					// GL_DRAW_BUFFER1
			DrawBuffer2,					// GL_DRAW_BUFFER2
			DrawBuffer3,					// GL_DRAW_BUFFER3
			DrawBuffer4,					// GL_DRAW_BUFFER4
			DrawBuffer5,					// GL_DRAW_BUFFER5
			DrawBuffer6,					// GL_DRAW_BUFFER6
			DrawBuffer7,					// GL_DRAW_BUFFER7
			DrawBuffer8,					// GL_DRAW_BUFFER8
			DrawBuffer9,					// GL_DRAW_BUFFER9
			DrawBuffer10,					// GL_DRAW_BUFFER10
			DrawBuffer11,					// GL_DRAW_BUFFER11
			DrawBuffer12,					// GL_DRAW_BUFFER12
			DrawBuffer13,					// GL_DRAW_BUFFER13
			DrawBuffer14,					// GL_DRAW_BUFFER14
			DrawBuffer15,					// GL_DRAW_BUFFER15
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Mask : uint32_t
		{
			ContextCoreProfileBit = 0x1,	// GL_CONTEXT_CORE_PROFILE_BIT
			ContextCompatProfileBit = 0x2,	// GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
			ContextFlagDebugBit	= 0x2,		// GL_CONTEXT_FLAG_DEBUG_BIT
		
			DepthBufferBit = 0x100,			// GL_DEPTH_BUFFER_BIT
			StencilBufferBit = 0x400,		// GL_STENCIL_BUFFER_BIT
			ColorBufferBit = 0x4000,		// GL_COLOR_BUFFER_BIT
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Graphics/GL.inl>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_GL_HPP_