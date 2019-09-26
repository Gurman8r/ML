#include <ML/Graphics/GL.hpp>

#define ML_GEN_GL_ENUM_INFO(TYPE) \
static constexpr bool value_at(int32_t i, TYPE & value) \
{ \
	return alg::value_at(i, value, TYPE##_values); \
} \
static constexpr int32_t index_of(TYPE value) \
{ \
	return alg::index_of(value, TYPE##_values); \
} \
static constexpr C_String name_of(TYPE value) \
{ \
	return alg::at_index(index_of(value), TYPE##_names); \
} \
static constexpr C_String raw_name_of(TYPE value) \
{ \
	return alg::at_index(index_of(value), TYPE##_raw_names); \
} \
static constexpr C_String desc_of(TYPE value) \
{ \
	return alg::at_index(index_of(value), TYPE##_descriptions); \
} \
static constexpr TYPE find_by_name(C_String value, TYPE dv = TYPE##_values[0]) \
{ \
	int32_t i = alg::index_of(value, TYPE##_names); \
	return ((i != -1) ? TYPE##_values[i] : dv); \
} \

namespace ml
{
	namespace GL
	{
		// GL::Flag
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Flag Flag_values[] = {
			Flag::CullFace,
			Flag::DepthTest,
			Flag::AlphaTest,
			Flag::Blend,
			Flag::Multisample,
			Flag::FramebufferSRGB,
			Flag::ScissorTest,
		};

		static constexpr C_String Flag_names[] = {
			"Cull Face",
			"Depth Test",
			"Alpha Test",
			"Blend",
			"Multisample",
			"Framebuffer sRGB",
			"Scissor Test",
		};

		static constexpr C_String Flag_raw_names[] = {
			"GL_CULL_FACE",
			"GL_DEPTH_TEST",
			"GL_ALPHA_TEST",
			"GL_BLEND",
			"GL_MULTISAMPLE",
			"GL_FRAMEBUFFER_SRGB",
			"GL_SCISSOR_TEST",
		};

		static constexpr C_String Flag_descriptions[] = {
			"If enabled, cull polygons based on their winding in window coordinates",
			"If enabled, do depth comparisons and update the depth buffer",
			"If enabled, do alpha testing",
			"If enabled, blend the computed fragment color values with the values in the color buffers",
			"If enabled, use multiple fragment samples in computing the final color of a pixel",
			"If enabled, and the value of the framebuffer colorID corresponding to the destination buffer is sRGB",
			"If enabled, discard fragments that are outside the scissor rectangle",
		};


		// GL::Sampler
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Sampler Sampler_values[] = {
			Sampler::Texture2D,
			Sampler::Texture3D,
			Sampler::TextureCubeMap,
		};

		static constexpr C_String Sampler_names[] = {
			"Texture 2D",
			"Texture 3D",
			"Texture Cube Map",
		};

		static constexpr C_String Sampler_raw_names[] = {
			"GL_TEXTURE_2D",
			"GL_TEXTURE_3D",
			"GL_TEXTURE_CUBE_MAP",
		};

		static constexpr C_String Sampler_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Target
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Target Target_values[] = {
			Target::ArrayBuffer,
			Target::ElementArrayBuffer,
			Target::ArrayBufferBinding,
			Target::Framebuffer,
			Target::Renderbuffer,
			Target::ProgramObject,
			Target::FramebufferRead,
			Target::FramebufferDraw,
			Target::ReadFramebufferBinding,
			Target::DrawFramebufferBinding,
			Target::CurrentProgram,
			Target::VertexArrayBinding,
			Target::TextureBinding2D,
			Target::SamplerBinding,
			Target::ActiveTexture,
		};

		static constexpr C_String Target_names[] = {
			"Array Buffer",
			"Element Array Buffer",
			"Array Buffer Binding",
			"Framebuffer",
			"Renderbuffer",
			"ProgramObject",
			"Framebuffer Read",
			"Framebuffer Draw",
			"Read Framebuffer Binding",
			"Draw Framebuffer Binding",
			"Current Program",
			"Vertex Array Binding",
			"Texture Binding 2D",
			"Sampler Binding",
			"Active Texture",
		};

		static constexpr C_String Target_raw_names[] = {
			"GL_ARRAY_BUFFER",
			"GL_ELEMENT_ARRAY_BUFFER",
			"GL_ARRAY_BUFFER_BINDING",
			"GL_FRAMEBUFFER",
			"GL_RENDERBUFFER",
			"GL_PROGRAM_OBJECT_ARB",
			"GL_READ_FRAMEBUFFER",
			"GL_DRAW_FRAMEBUFFER",
			"GL_READ_FRAMEBUFFER_BINDING",
			"GL_DRAW_FRAMEBUFFER_BINDING",
			"GL_CURRENT_PROGRAM",
			"GL_VERTEX_ARRAY_BINDING",
			"GL_TEXTURE_BINDING_2D",
			"GL_SAMPLER_BINDING",
			"GL_ACTIVE_TEXTURE",
		};

		static constexpr C_String Target_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Usage
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Usage Usage_values[] = {
			Usage::StreamDraw,
			Usage::StaticDraw,
			Usage::DynamicDraw,
		};

		static constexpr C_String Usage_names[] = {
			"Stream Draw",
			"Static Draw",
			"Dynamic Draw",
		};

		static constexpr C_String Usage_raw_names[] = {
			"GL_STREAM_DRAW",
			"GL_STATIC_DRAW",
			"GL_DYNAMIC_DRAW",
		};

		static constexpr C_String Usage_descriptions[] = {
			"The data will be modified once and used at most a few times",
			"The data will be modified once and used many times",
			"The data will be modified repeatedly and used many times",
		};


		// GL::Err
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Err Err_values[] = {
			Err::NoError,
			Err::InvalidEnum,
			Err::InvalidValue,
			Err::InvalidOperation,
			Err::StackOverflow,
			Err::StackUnderflow,
			Err::OutOfMemory,
			Err::InvalidFramebufferOperation,
		};

		static constexpr C_String Err_names[] = {
			"No Error",
			"Invalid Enum",
			"Invalid Value",
			"Invalid Operation",
			"Stack Overflow",
			"Stack Underflow",
			"Out Of Memory",
			"Invalid Framebuffer Operation",
		};

		static constexpr C_String Err_raw_names[] = {
			"GL_NO_ERROR",
			"GL_INVALID_ENUM",
			"GL_INVALID_VALUE",
			"GL_INVALID_OPERATION",
			"GL_STACK_OVERFLOW",
			"GL_STACK_UNDERFLOW",
			"GL_OUT_OF_MEMORY",
			"GL_INVALID_FRAMEBUFFER_OPERATION",
		};

		static constexpr C_String Err_descriptions[] = {
			"No Error Description",
			"An unacceptable value has been specified for an enumerated argument",
			"A numeric argument is out of range",
			"The specified operation is not allowed in the current state",
			"This command would cause a stack overflow",
			"This command would cause a stack underflow",
			"There is not enough memory left to execute the command",
			"The object bound to framebuffer binding is not \'framebuffer complete\'"
		};


		// GL::StringID
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr StringID StringID_values[] = {
			StringID::Vendor,
			StringID::Renderer,
			StringID::Version,
			StringID::Extensions,
			StringID::ShadingLanguageVersion,
		};

		static constexpr C_String StringID_names[] = {
			"Vendor",
			"Renderer",
			"Version",
			"Extensions",
			"Shading Language Version",
		};

		static constexpr C_String StringID_raw_names[] = {
			"GL_VENDOR",
			"GL_RENDERER",
			"GL_VERSION",
			"GL_EXTENSIONS",
			"GL_SHADING_LANGUAGE_VERSION",
		};

		static constexpr C_String StringID_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::IntID
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr IntID IntID_values[] = {
			IntID::MajorVersion,
			IntID::MinorVersion,
			IntID::NumExtensions,
			IntID::ContextFlags,
			IntID::ContextProfileMask,
			IntID::MaxTextureSize,
			IntID::MaxCombTexImgUnits,
			IntID::InfoLogLength,
			IntID::PolygonMode,
			IntID::Viewport,
			IntID::ScissorBox,
			IntID::BlendEquationRGB,
			IntID::BlendEquationAlpha,
			IntID::BlendDestRGB,
			IntID::BlendSourceRGB,
			IntID::BlendDestAlpha,
			IntID::BlendSourceAlpha,
		};

		static constexpr C_String IntID_names[] = {
			"Major Version",
			"Minor Version",
			"Num Extensions",
			"Context Flags",
			"Context Profile Mask",
			"Max Texture Size",
			"Max Combined Texture Image Units",
			"Info Log Length",
			"Polygon Mode",
			"Viewport",
			"Scissor Box",
			"Blend Equation RGB",
			"Blend Equation Alpha",
			"Blend Dest RGB",
			"Blend Source RGB",
			"Blend Dest Alpha",
			"Blend Source Alpha",
		};

		static constexpr C_String IntID_raw_names[] = {
			"GL_MAJOR_VERSION",
			"GL_MINOR_VERSION",
			"GL_NUM_EXTENSIONS",
			"GL_CONTEXT_FLAGS",
			"GL_CONTEXT_PROFILE_MASK",
			"GL_MAX_TEXTURE_SIZE",
			"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
			"GL_INFO_LOG_LENGTH",
			"GL_POLYGON_MODE",
			"GL_VIEWPORT",
			"GL_SCISSOR_BOX",
			"GL_BLEND_EQUATION_RGB",
			"GL_BLEND_EQUATION_ALPHA",
			"GL_BLEND_DST_RGB",
			"GL_BLEND_SRC_RGB",
			"GL_BLEND_DST_ALPHA",
			"GL_BLEND_SRC_ALPHA",
		};

		static constexpr C_String IntID_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Clip
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Clip Clip_values[] = {
			Clip::LowerLeft,
			Clip::UpperLeft,
			Clip::ClipOrigin,
		};

		static constexpr C_String Clip_names[] = {
			"Lower Left",
			"Upper Left",
			"Clip Origin",
		};

		static constexpr C_String Clip_raw_names[] = {
			"GL_LOWER_LEFT",
			"GL_UPPER_LEFT",
			"GL_CLIP_ORIGIN",
		};

		static constexpr C_String Clip_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Status
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Status Status_values[] = {
			Status::ObjectDeleteStatus,
			Status::ObjectCompileStatus,
			Status::ObjectLinkStatus,
			Status::FramebufferComplete,
		};

		static constexpr C_String Status_names[] = {
			"Object Delete Status",
			"Object Compile Status",
			"Object Link Status",
			"Framebuffer Complete",
		};

		static constexpr C_String Status_raw_names[] = {
			"GL_OBJECT_DELETE_STATUS_ARB",
			"GL_OBJECT_COMPILE_STATUS_ARB",
			"GL_OBJECT_LINK_STATUS_ARB",
			"GL_FRAMEBUFFER_COMPLETE",
		};

		static constexpr C_String Status_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};

		
		// GL::ShaderType
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr ShaderType ShaderType_values[] = {
			ShaderType::FragmentShader,
			ShaderType::VertexShader,
			ShaderType::GeometryShader,
		};

		static constexpr C_String ShaderType_names[] = {
			"Fragment Shader",
			"Vertex Shader",
			"Geometry Shader",
		};

		static constexpr C_String ShaderType_raw_names[] = {
			"GL_FRAGMENT_SHADER",
			"GL_VERTEX_SHADER",
			"GL_GEOMETRY_SHADER",
		};

		static constexpr C_String ShaderType_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
		};

		
		// GL::Mode
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Mode Mode_values[] = {
			Mode::Points,
			Mode::Lines,
			Mode::LineLoop,
			Mode::LineStrip,
			Mode::Triangles,
			Mode::TriangleStrip,
			Mode::TriangleFan,
			Mode::Fill,
		};

		static constexpr C_String Mode_names[] = {
			"Points",
			"Lines",
			"Line Loop",
			"Line Strip",
			"Triangles",
			"Triangle Strip",
			"Triangle Fan",
			"Fill",
		};

		static constexpr C_String Mode_raw_names[] = {
			"GL_POINTS",
			"GL_LINES",
			"GL_LINE_LOOP",
			"GL_LINE_STRIP",
			"GL_TRIANGLES",
			"GL_TRIANGLE_STRIP",
			"GL_TRIANGLE_FAN",
			"GL_FILL",
		};

		static constexpr C_String Mode_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Equation
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Equation Equation_values[] = {
			Equation::Multiply,
			Equation::Add,
			Equation::Subtract,
			Equation::FuncAdd,
		};

		static constexpr C_String Equation_names[] = {
			"Multiply",
			"Add",
			"Subtract",
			"Func Add",
		};

		static constexpr C_String Equation_raw_names[] = {
			"GL_MULT",
			"GL_ADD",
			"GL_SUBTRACT",
			"GL_FUNC_ADD",
		};

		static constexpr C_String Equation_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Predicate
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr const Predicate Predicate_values[] = {
			Predicate::Never,
			Predicate::Less,
			Predicate::Equal,
			Predicate::LEqual,
			Predicate::Greater,
			Predicate::NotEqual,
			Predicate::GEqual,
			Predicate::Always,
		};

		static constexpr C_String Predicate_names[] = {
			"Never",
			"Less",
			"Equal",
			"Less / Equal",
			"Greater",
			"Not Equal",
			"Greater / Equal",
			"Always",
		};

		static constexpr C_String Predicate_raw_names[] = {
			"GL_NEVER",
			"GL_LESS",
			"GL_EQUAL",
			"GL_LEQUAL",
			"GL_GREATER",
			"GL_NOTEQUAL",
			"GL_GEQUAL",
			"GL_ALWAYS",
		};

		static constexpr C_String Predicate_descriptions[] = {
			"Never passes",
			"Passes if the incoming value is less than the stored value",
			"Passes if the incoming value is equal to the stored value",
			"Passes if the incoming value is less than or equal to the stored value",
			"Passes if the incoming value is greater than the stored value",
			"Passes if the incoming value is not equal to the stored value",
			"Passes if the incoming value is greater than or equal to the stored value",
			"Always passes",
		};


		// GL::Factor
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Factor Factor_values[] = {
			Factor::SrcColor,
			Factor::OneMinusSrcColor,
			Factor::SrcAlpha,
			Factor::OneMinusSrcAlpha,
			Factor::DstAlpha,
			Factor::OneMinusDstAlpha,
			Factor::DstColor,
			Factor::OneMinusDstColor,
			Factor::SrcAlphaSaturate,
		};

		static constexpr C_String Factor_names[] = {
			"Source Color",
			"One Minus Source Color",
			"Source Alpha",
			"One Minus Source Alpha",
			"Dest Alpha",
			"One Minus Dest Alpha",
			"Dest Color",
			"One Minus Dest Color",
			"Source Alpha Saturate",
		};

		static constexpr C_String Factor_raw_names[] = {
			"GL_SRC_COLOR",
			"GL_ONE_MINUS_SRC_COLOR",
			"GL_SRC_ALPHA",
			"GL_ONE_MINUS_SRC_ALPHA",
			"GL_DST_ALPHA",
			"GL_ONE_MINUS_DST_ALPHA",
			"GL_DST_COLOR",
			"GL_ONE_MINUS_DST_COLOR",
			"GL_SRC_ALPHA_SATURATE",
		};

		static constexpr C_String Factor_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Face
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr const Face Face_values[] = {
			Face::FrontLeft,
			Face::FrontRight,
			Face::BackLeft,
			Face::BackRight,
			Face::Front,
			Face::Back,
			Face::Left,
			Face::Right,
			Face::FrontAndBack,
		};

		static constexpr C_String Face_names[] = {
			"Front / Left",
			"Front / Right",
			"Back / Left",
			"Back / Right",
			"Front",
			"Back",
			"Left",
			"Right",
			"Front & Back",
		};

		static constexpr C_String Face_raw_names[] = {
			"GL_FRONT_LEFT",
			"GL_FRONT_RIGHT",
			"GL_BACK_LEFT",
			"GL_BACK_RIGHT",
			"GL_FRONT",
			"GL_BACK",
			"GL_LEFT",
			"GL_RIGHT",
			"GL_FRONT_AND_BACK",
		};

		static constexpr C_String Face_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};

		
		// GL::Type
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Type Type_values[] = {
			Type::Byte,
			Type::UnsignedByte,
			Type::Short,
			Type::UnsignedShort,
			Type::Int,
			Type::UnsignedInt,
			Type::Float,
			Type::HalfFloat,
		};

		static constexpr C_String Type_names[] = {
			"Byte",
			"Unsigned Byte",
			"Short",
			"Unsigned Short",
			"Int",
			"Unsigned Int",
			"Float",
			"Half Float",
		};

		static constexpr C_String Type_raw_names[] = {
			"GL_BYTE",
			"GL_UNSIGNED_BYTE",
			"GL_SHORT",
			"GL_UNSIGNED_SHORT",
			"GL_INT",
			"GL_UNSIGNED_INT",
			"GL_FLOAT",
			"GL_HALF_FLOAT",
		};

		static constexpr C_String Type_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Format
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Format Format_values[] = {
			Format::Red,
			Format::Green,
			Format::Blue,
			Format::Alpha,
			Format::RGB,
			Format::RGBA,
			Format::Luminance,
			Format::Luminance_Alpha,
			Format::SRGB,
			Format::SRGB8,
			Format::SRGB_Alpha,
			Format::SRGB8_Alpha8,
			Format::SLuminance_Alpha,
			Format::SLuminance8_Alpha8,
			Format::SLuminance,
			Format::SLuminance8,
			Format::Depth24_Stencil8,
		};

		static constexpr C_String Format_names[] = {
			"Red",
			"Green",
			"Blue",
			"Alpha",
			"RGB",
			"RGBA",
			"Luminance",
			"Luminance / Alpha",
			"S-RGB",
			"S-RGB8",
			"S-RGB / Alpha",
			"S-RGB8 / Alpha 8",
			"S-Luminance / Alpha",
			"S-Luminance 8 / Alpha 8",
			"S-Luminance",
			"S-Luminance 8",
			"Depth 24 / Stencil 8",
		};

		static constexpr C_String Format_raw_names[] = {
			"GL_RED",
			"GL_GREEN",
			"GL_BLUE",
			"GL_ALPHA",
			"GL_RGB",
			"GL_RGBA",
			"GL_LUMINANCE",
			"GL_LUMINANCE_ALPHA",
			"GL_SRGB",
			"GL_SRGB8",
			"GL_SRGB_ALPHA",
			"GL_SRGB8_ALPHA8",
			"GL_SLUMINANCE_ALPHA",
			"GL_SLUMINANCE8_ALPHA8",
			"GL_SLUMINANCE",
			"GL_SLUMINANCE8",
			"GL_DEPTH24_STENCIL8",
		};

		static constexpr C_String Format_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::FrameID
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr FrameID FrameID_values[] = {
			GL::FrameID::ColorEncoding,
			GL::FrameID::ComponentType,
			GL::FrameID::RedSize,
			GL::FrameID::GreenSize,
			GL::FrameID::BlueSize,
			GL::FrameID::AlphaSize,
			GL::FrameID::DepthSize,
			GL::FrameID::StencilSize,
			GL::FrameID::FramebufferDefault,
			GL::FrameID::FramebufferUndefined,
			GL::FrameID::DepthStencil,
		};

		static constexpr C_String FrameID_names[] = {
			"Color Encoding",
			"Component Type",
			"Red Size",
			"Green Size",
			"Blue Size",
			"Alpha Size",
			"Depth Size",
			"Stencil Size",
			"Framebuffer Default",
			"Framebuffer Undefined",
			"Depth Stencil",
		};

		static constexpr C_String FrameID_raw_names[] = {
			"GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING",
			"GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE",
			"GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE",
			"GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE",
			"GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE",
			"GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE",
			"GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE",
			"GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE",
			"GL_FRAMEBUFFER_DEFAULT",
			"GL_FRAMEBUFFER_UNDEFINED",
			"GL_DEPTH_STENCIL_ATTACHMENT",
		};

		static constexpr C_String FrameID_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::TexParam
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr TexParam TexParam_values[] = {
			TexParam::Nearest,
			TexParam::Linear,
			TexParam::NearestMipmapNearest,
			TexParam::LinearMipmapNearest,
			TexParam::NearestMipmapLinear,
			TexParam::LinearMipmapLinear,
			TexParam::TexMagFilter,
			TexParam::TexMinFilter,
			TexParam::TexWrapS,
			TexParam::TexWrapT,
			TexParam::TexWrapR,
			TexParam::Clamp,
			TexParam::Repeat,
			TexParam::ClampToEdge,
			TexParam::MinLOD,
			TexParam::MaxLOD,
			TexParam::BaseLevel,
			TexParam::CubeMap_Positive_X,
			TexParam::CubeMap_Negative_X,
			TexParam::CubeMap_Positive_Y,
			TexParam::CubeMap_Negative_Y,
			TexParam::CubeMap_Positive_Z,
			TexParam::CubeMap_Negative_Z,
		};

		static constexpr C_String TexParam_names[] = {
			"Nearest",
			"Linear",
			"Nearest Mipmap Nearest",
			"Linear Mipmap Nearest",
			"Nearest Mipmap Linear",
			"Linear Mipmap Linear",
			"Texture Mag Filter",
			"Texture Min Filter",
			"Texture Wrap S",
			"Texture Wrap T",
			"Texture Wrap R",
			"Clamp",
			"Repeat",
			"Clamp To Edge",
			"Min LOD",
			"Max LOD",
			"Base Level",
			"Max Level",
			"CubeMap Positive X",
			"CubeMap Negative X",
			"CubeMap Positive Y",
			"CubeMap Negative Y",
			"CubeMap Positive Z",
			"CubeMap Negative Z",
		};

		static constexpr C_String TexParam_raw_names[] = {
			"GL_NEAREST",
			"GL_LINEAR",
			"GL_NEAREST_MIPMAP_NEAREST",
			"GL_LINEAR_MIPMAP_NEAREST",
			"GL_NEAREST_MIPMAP_LINEAR",
			"GL_LINEAR_MIPMAP_LINEAR",
			"GL_TEXTURE_MAG_FILTER",
			"GL_TEXTURE_MIN_FILTER",
			"GL_TEXTURE_WRAP_S",
			"GL_TEXTURE_WRAP_T",
			"GL_TEXTURE_WRAP_R",
			"GL_CLAMP",
			"GL_REPEAT",
			"GL_CLAMP_TO_EDGE",
			"GL_TEXTURE_MIN_LOD",
			"GL_TEXTURE_MAX_LOD",
			"GL_TEXTURE_BASE_LEVEL",
			"GL_TEXTURE_MAX_LEVEL",
			"GL_TEXTURE_CUBE_MAP_POSITIVE_X",
			"GL_TEXTURE_CUBE_MAP_NEGATIVE_X",
			"GL_TEXTURE_CUBE_MAP_POSITIVE_Y",
			"GL_TEXTURE_CUBE_MAP_NEGATIVE_Y",
			"GL_TEXTURE_CUBE_MAP_POSITIVE_Z",
			"GL_TEXTURE_CUBE_MAP_NEGATIVE_Z",
		};

		static constexpr C_String TexParam_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Pack
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Pack Pack_values[] = {
			Pack::UnpackSwapBytes,
			Pack::UnpackLsbFirst,
			Pack::UnpackRowLength,
			Pack::UnpackSkipRows,
			Pack::UnpackSkipPixels,
			Pack::UnpackAlignment,
			Pack::PackSwapBytes,
			Pack::PackLsbFirst,
			Pack::PackRowLength,
			Pack::PackSkipRows,
			Pack::PackSkipPixels,
			Pack::PackAlignment,
		};

		static constexpr C_String Pack_names[] = {
			"Unpack Swap Bytes",
			"Unpack Lsb First",
			"Unpack Row Length",
			"Unpack Skip Rows",
			"Unpack Skip Pixels",
			"Unpack Alignment",
			"Pack Swap Bytes",
			"Pack Lsb First",
			"Pack Row Length",
			"Pack Skip Rows",
			"Pack Skip Pixels",
			"Pack Alignment",
		};

		static constexpr C_String Pack_raw_names[] = {
			"GL_UNPACK_SWAP_BYTES",
			"GL_UNPACK_LSB_FIRST",
			"GL_UNPACK_ROW_LENGTH",
			"GL_UNPACK_SKIP_ROWS",
			"GL_UNPACK_SKIP_PIXELS",
			"GL_UNPACK_ALIGNMENT",
			"GL_PACK_SWAP_BYTES",
			"GL_PACK_LSB_FIRST",
			"GL_PACK_ROW_LENGTH",
			"GL_PACK_SKIP_ROWS",
			"GL_PACK_SKIP_PIXELS",
			"GL_PACK_ALIGNMENT",
		};

		static constexpr C_String Pack_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::Attachment
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr ColorID ColorID_values[] = {
			ColorID::ColorAttachment0,
			ColorID::ColorAttachment1,
			ColorID::ColorAttachment2,
			ColorID::ColorAttachment3,
			ColorID::ColorAttachment4,
			ColorID::ColorAttachment5,
			ColorID::ColorAttachment6,
			ColorID::ColorAttachment7,
			ColorID::ColorAttachment8,
			ColorID::ColorAttachment9,
			ColorID::ColorAttachment10,
			ColorID::ColorAttachment11,
			ColorID::ColorAttachment12,
			ColorID::ColorAttachment13,
			ColorID::ColorAttachment14,
			ColorID::ColorAttachment15,
		};

		static constexpr C_String ColorID_names[] = {
			"Color Attachment 0",
			"Color Attachment 1",
			"Color Attachment 2",
			"Color Attachment 3",
			"Color Attachment 4",
			"Color Attachment 5",
			"Color Attachment 6",
			"Color Attachment 7",
			"Color Attachment 8",
			"Color Attachment 9",
			"Color Attachment 10",
			"Color Attachment 11",
			"Color Attachment 12",
			"Color Attachment 13",
			"Color Attachment 14",
			"Color Attachment 15",
		};

		static constexpr C_String ColorID_raw_names[] = {
			"GL_COLOR_ATTACHMENT0",
			"GL_COLOR_ATTACHMENT1",
			"GL_COLOR_ATTACHMENT2",
			"GL_COLOR_ATTACHMENT3",
			"GL_COLOR_ATTACHMENT4",
			"GL_COLOR_ATTACHMENT5",
			"GL_COLOR_ATTACHMENT6",
			"GL_COLOR_ATTACHMENT7",
			"GL_COLOR_ATTACHMENT8",
			"GL_COLOR_ATTACHMENT9",
			"GL_COLOR_ATTACHMENT10",
			"GL_COLOR_ATTACHMENT11",
			"GL_COLOR_ATTACHMENT12",
			"GL_COLOR_ATTACHMENT13",
			"GL_COLOR_ATTACHMENT14",
			"GL_COLOR_ATTACHMENT15",
		};

		static constexpr C_String ColorID_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::TexID
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr TexID TexID_values[] = {
			TexID::Texture0,
			TexID::Texture1,
			TexID::Texture2,
			TexID::Texture3,
			TexID::Texture4,
			TexID::Texture5,
			TexID::Texture6,
			TexID::Texture7,
			TexID::Texture8,
			TexID::Texture9,
			TexID::Texture10,
			TexID::Texture11,
			TexID::Texture12,
			TexID::Texture13,
			TexID::Texture14,
			TexID::Texture15,
			TexID::Texture16,
			TexID::Texture17,
			TexID::Texture18,
			TexID::Texture19,
			TexID::Texture20,
			TexID::Texture21,
			TexID::Texture22,
			TexID::Texture23,
			TexID::Texture24,
			TexID::Texture25,
			TexID::Texture26,
			TexID::Texture27,
			TexID::Texture28,
			TexID::Texture29,
			TexID::Texture30,
			TexID::Texture31,
		};

		static constexpr C_String TexID_names[] = {
			"Texture 0",
			"Texture 1",
			"Texture 2",
			"Texture 3",
			"Texture 4",
			"Texture 5",
			"Texture 6",
			"Texture 7",
			"Texture 8",
			"Texture 9",
			"Texture 10",
			"Texture 11",
			"Texture 12",
			"Texture 13",
			"Texture 14",
			"Texture 15",
			"Texture 16",
			"Texture 17",
			"Texture 18",
			"Texture 19",
			"Texture 20",
			"Texture 21",
			"Texture 22",
			"Texture 23",
			"Texture 24",
			"Texture 25",
			"Texture 26",
			"Texture 27",
			"Texture 28",
			"Texture 29",
			"Texture 30",
			"Texture 31",
		};

		static constexpr C_String TexID_raw_names[] = {
			"GL_TEXTURE0",
			"GL_TEXTURE1",
			"GL_TEXTURE2",
			"GL_TEXTURE3",
			"GL_TEXTURE4",
			"GL_TEXTURE5",
			"GL_TEXTURE6",
			"GL_TEXTURE7",
			"GL_TEXTURE8",
			"GL_TEXTURE9",
			"GL_TEXTURE10",
			"GL_TEXTURE11",
			"GL_TEXTURE12",
			"GL_TEXTURE13",
			"GL_TEXTURE14",
			"GL_TEXTURE15",
			"GL_TEXTURE16",
			"GL_TEXTURE17",
			"GL_TEXTURE18",
			"GL_TEXTURE19",
			"GL_TEXTURE20",
			"GL_TEXTURE21",
			"GL_TEXTURE22",
			"GL_TEXTURE23",
			"GL_TEXTURE24",
			"GL_TEXTURE25",
			"GL_TEXTURE26",
			"GL_TEXTURE27",
			"GL_TEXTURE28",
			"GL_TEXTURE29",
			"GL_TEXTURE30",
			"GL_TEXTURE31",
		};

		static constexpr C_String TexID_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};


		// GL::DrawBuffer
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr DrawBuffer DrawBuffer_values[] = {
			DrawBuffer::DrawBuffer0,
			DrawBuffer::DrawBuffer1,
			DrawBuffer::DrawBuffer2,
			DrawBuffer::DrawBuffer3,
			DrawBuffer::DrawBuffer4,
			DrawBuffer::DrawBuffer5,
			DrawBuffer::DrawBuffer6,
			DrawBuffer::DrawBuffer7,
			DrawBuffer::DrawBuffer8,
			DrawBuffer::DrawBuffer9,
			DrawBuffer::DrawBuffer10,
			DrawBuffer::DrawBuffer11,
			DrawBuffer::DrawBuffer12,
			DrawBuffer::DrawBuffer13,
			DrawBuffer::DrawBuffer14,
			DrawBuffer::DrawBuffer15,
		};

		static constexpr C_String DrawBuffer_names[] = {
			"Draw Buffer 0",
			"Draw Buffer 1",
			"Draw Buffer 2",
			"Draw Buffer 3",
			"Draw Buffer 4",
			"Draw Buffer 5",
			"Draw Buffer 6",
			"Draw Buffer 7",
			"Draw Buffer 8",
			"Draw Buffer 9",
			"Draw Buffer 10",
			"Draw Buffer 11",
			"Draw Buffer 12",
			"Draw Buffer 13",
			"Draw Buffer 14",
			"Draw Buffer 15",
		};

		static constexpr C_String DrawBuffer_raw_names[] = {
			"GL_DRAW_BUFFER0",
			"GL_DRAW_BUFFER1",
			"GL_DRAW_BUFFER2",
			"GL_DRAW_BUFFER3",
			"GL_DRAW_BUFFER4",
			"GL_DRAW_BUFFER5",
			"GL_DRAW_BUFFER6",
			"GL_DRAW_BUFFER7",
			"GL_DRAW_BUFFER8",
			"GL_DRAW_BUFFER9",
			"GL_DRAW_BUFFER10",
			"GL_DRAW_BUFFER11",
			"GL_DRAW_BUFFER12",
			"GL_DRAW_BUFFER13",
			"GL_DRAW_BUFFER14",
			"GL_DRAW_BUFFER15",
		};

		static constexpr C_String DrawBuffer_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};

		// GL::Mask
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Mask Mask_values[] = {
			Mask::ContextCoreProfileBit,
			Mask::ContextCompatProfileBit,
			Mask::ContextFlagDebugBit,
			Mask::DepthBufferBit,
			Mask::StencilBufferBit,
			Mask::ColorBufferBit,
		};

		static constexpr C_String Mask_names[] = {
			"Context Core Profile Bit",
			"Context Compat Profile Bit",
			"Context Flag Debug Bit",
			"Depth Buffer Bit",
			"Stencil Buffer Bit",
			"Color Buffer Bit",
		};

		static constexpr C_String Mask_raw_names[] = {
			"GL_CONTEXT_CORE_PROFILE_BIT",
			"GL_CONTEXT_COMPATIBILITY_PROFILE_BIT",
			"GL_CONTEXT_FLAG_DEBUG_BIT",
			"GL_DEPTH_BUFFER_BIT",
			"GL_STENCIL_BUFFER_BIT",
			"GL_COLOR_BUFFER_BIT",
		};

		static constexpr C_String Mask_descriptions[] = {
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
			"No Description",
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

// OpenGL Enum Functions
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	namespace GL
	{
		/* * * * * * * * * * * * * * * * * * * * */

		ML_GEN_GL_ENUM_INFO(Flag);
		ML_GEN_GL_ENUM_INFO(Sampler);
		ML_GEN_GL_ENUM_INFO(Target);
		ML_GEN_GL_ENUM_INFO(Usage);
		ML_GEN_GL_ENUM_INFO(Err);
		ML_GEN_GL_ENUM_INFO(StringID);
		ML_GEN_GL_ENUM_INFO(IntID);
		ML_GEN_GL_ENUM_INFO(Clip);
		ML_GEN_GL_ENUM_INFO(Status);
		ML_GEN_GL_ENUM_INFO(ShaderType);
		ML_GEN_GL_ENUM_INFO(Mode);
		ML_GEN_GL_ENUM_INFO(Equation);
		ML_GEN_GL_ENUM_INFO(Predicate);
		ML_GEN_GL_ENUM_INFO(Factor);
		ML_GEN_GL_ENUM_INFO(Face);
		ML_GEN_GL_ENUM_INFO(Type);
		ML_GEN_GL_ENUM_INFO(Format);
		ML_GEN_GL_ENUM_INFO(FrameID);
		ML_GEN_GL_ENUM_INFO(TexParam);
		ML_GEN_GL_ENUM_INFO(Pack);
		ML_GEN_GL_ENUM_INFO(ColorID);
		ML_GEN_GL_ENUM_INFO(TexID);
		ML_GEN_GL_ENUM_INFO(DrawBuffer);
		ML_GEN_GL_ENUM_INFO(Mask);

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr T value_at(int32_t index, T dv = (T)0)
		{
			::ml::GL::value_at(index, dv);
			return dv;
		}

		template <class T> static constexpr int32_t index_of(T value)
		{
			return ::ml::GL::index_of(value);
		}

		template <class T> static constexpr C_String name_of(T value)
		{
			return ::ml::GL::name_of(value);
		}

		template <class T> static constexpr C_String raw_name_of(T value)
		{
			return ::ml::GL::raw_name_of(value);
		}

		template <class T> static constexpr C_String desc_of(T value)
		{
			return ::ml::GL::desc_of(value);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	inline ML_SERIALIZE(Ostream & out, const GL::Flag & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Sampler & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Target & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Usage & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Err & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::StringID & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::IntID & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Clip & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Status & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::ShaderType & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Mode & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Equation & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Predicate & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Factor & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Face & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Type & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Format & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::FrameID & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::TexParam & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Pack & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::ColorID & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::TexID & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::DrawBuffer & value) { return out << GL::name_of(value); }
	inline ML_SERIALIZE(Ostream & out, const GL::Mask & value) { return out << GL::name_of(value); }
}