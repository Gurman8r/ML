#include <ML/Graphics/GL.hpp>
#include <ML/Core/Preprocessor.hpp>
#include <ML/Core/FMT.hpp>

// Detail
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	namespace GL
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		namespace detail
		{
			template <
				class T, size_t N
			> static constexpr auto index_of(const T value, const T(&arr)[N])
			{
				for (int32_t i = 0; (&arr[i]) != (&arr[N]); i++)
				{
					if (arr[i] == value)
					{
						return i;
					}
				}
				return -1;
			}

			template <
				class T, size_t N
			> static constexpr bool value_at(const int32_t i, T & value, const T(&arr)[N])
			{
				if (i < (int32_t)(sizeof(arr) / sizeof(*arr)))
				{
					value = arr[i];
					return true;
				}
				return false;
			}

			template <
				class Out, class In, class Enum
			> static constexpr auto cast_enum(const In & value, const Enum & maxValue)
			{
				// only works for enums with contiguous values
				return (Out)(((uint32_t)maxValue - (uint32_t)value) + 1);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}


// Values / Names
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	namespace GL
	{
		// GL::Flag
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Flag Flag_values[] = {
			Flag::CullFace,
			Flag::DepthTest,
			Flag::AlphaTest,
			Flag::Blend,
			Flag::Multisample,
			Flag::FramebufferSRGB,
			Flag::ScissorTest,
		};

		static constexpr CString Flag_names[] = {
			"Cull Face",
			"Depth Test",
			"Alpha Test",
			"Blend",
			"Multisample",
			"Framebuffer SRGB",
			"Scissor Test",
		};

		static constexpr bool getEnum(const int32_t i, Flag & value)
		{
			return detail::value_at(i, value, Flag_values);
		}

		static constexpr int32_t indexOf(const Flag value)
		{
			return detail::index_of(value, Flag_values);
		}

		static constexpr CString nameOf(const Flag value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Flag_names[i] : "";
		}

		
		// GL::Target
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Target Target_values[] = {
			Target::Texture2D,
			Target::Texture3D,
			Target::TextureCubeMap,
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

		static constexpr CString Target_names[] = {
			"Texture 2D",
			"Texture 3D",
			"Texture Cube Map",
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

		static constexpr bool getEnum(const int32_t i, Target & value)
		{
			return detail::value_at(i, value, Target_values);
		}

		static constexpr int32_t indexOf(const Target value)
		{
			return detail::index_of(value, Target_values);
		}

		static constexpr CString nameOf(const Target value)
		{
			const int32_t i = detail::index_of(value, Target_values);
			return (i >= 0) ? Target_names[i] : "";
		}


		// GL::Usage
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Usage Usage_values[] = {
			Usage::StreamDraw,
			Usage::StaticDraw,
			Usage::DynamicDraw,
		};

		static constexpr CString Usage_names[] = {
			"Stream Draw",
			"Static Draw",
			"Dynamic Draw",
		};

		static constexpr bool getEnum(const int32_t i, Usage & value)
		{
			return detail::value_at(i, value, Usage_values);
		}

		static constexpr int32_t indexOf(const Usage value)
		{
			return detail::index_of(value, Usage_values);
		}

		static constexpr CString nameOf(const Usage value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Usage_names[i] : "";
		}


		// GL::Err
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString Err_names[] = {
			"No Error",
			"Invalid Enum",
			"Invalid Value",
			"Invalid Operation",
			"Stack Overflow",
			"Stack Underflow",
			"Out Of Memory",
			"Invalid Framebuffer Operation",
		};

		static constexpr bool getEnum(const int32_t i, Err & value)
		{
			return detail::value_at(i, value, Err_values);
		}

		static constexpr int32_t indexOf(const Err value)
		{
			return detail::index_of(value, Err_values);
		}

		static constexpr CString nameOf(const Err value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Err_names[i] : "";
		}

		
		// GL::StringID
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr StringID StringID_values[] = {
			Vendor,
			Renderer,
			Version,
			Extensions,
			ShadingLanguageVersion,
		};

		static constexpr CString StringID_names[] = {
			"Vendor",
			"Renderer",
			"Version",
			"Extensions",
			"Shading Language Version",
		};

		static constexpr bool getEnum(const int32_t i, StringID & value)
		{
			return detail::value_at(i, value, StringID_values);
		}

		static constexpr int32_t indexOf(const StringID value)
		{
			return detail::index_of(value, StringID_values);
		}

		static constexpr CString nameOf(const StringID value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? StringID_names[i] : "";
		}
		

		// GL::IntID
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr IntID IntID_values[] = {
			MajorVersion,
			MinorVersion,
			NumExtensions,
			ContextFlags,
			ContextProfileMask,
			MaxTextureSize,
			MaxCombTexImgUnits,
			InfoLogLength,
			PolygonMode,
			Viewport,
			ScissorBox,
			BlendEquationRGB,
			BlendEquationAlpha,
			BlendDestRGB,
			BlendSourceRGB,
			BlendDestAlpha,
			BlendSourceAlpha,
		};

		static constexpr CString IntID_names[] = {
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

		static constexpr bool getEnum(const int32_t i, IntID & value)
		{
			return detail::value_at(i, value, IntID_values);
		}

		static constexpr int32_t indexOf(const IntID value)
		{
			return detail::index_of(value, IntID_values);
		}

		static constexpr CString nameOf(const IntID value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? IntID_names[i] : "";
		}
		

		// GL::ClipControl
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr ClipControl ClipControl_values[] = {
			LowerLeft,
			UpperLeft,
			ClipOrigin,
		};

		static constexpr CString ClipControl_names[] = {
			"Lower Left",
			"Upper Left",
			"Clip Origin",
		};

		static constexpr bool getEnum(const int32_t i, ClipControl & value)
		{
			return detail::value_at(i, value, ClipControl_values);
		}

		static constexpr int32_t indexOf(const ClipControl value)
		{
			return detail::index_of(value, ClipControl_values);
		}

		static constexpr CString nameOf(const ClipControl value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? ClipControl_names[i] : "";
		}
		

		// GL::Status
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Status Status_values[] = {
			Status::ObjectDeleteStatus,
			Status::ObjectCompileStatus,
			Status::ObjectLinkStatus,
			Status::FramebufferComplete,
		};

		static constexpr CString Status_names[] = {
			"Object Delete Status",
			"Object Compile Status",
			"Object Link Status",
			"Framebuffer Complete",
		};

		static constexpr bool getEnum(const int32_t i, Status & value)
		{
			return detail::value_at(i, value, Status_values);
		}

		static constexpr int32_t indexOf(const Status value)
		{
			return detail::index_of(value, Status_values);
		}

		static constexpr CString nameOf(const Status value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Status_names[i] : "";
		}
		
		// GL::ShaderType
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr ShaderType ShaderType_values[] = {
			ShaderType::FragmentShader,
			ShaderType::VertexShader,
			ShaderType::GeometryShader,
		};

		static constexpr CString ShaderType_names[] = {
			"Fragment Shader",
			"Vertex Shader",
			"Geometry Shader",
		};

		static constexpr bool getEnum(const int32_t i, ShaderType & value)
		{
			return detail::value_at(i, value, ShaderType_values);
		}

		static constexpr int32_t indexOf(const ShaderType value)
		{
			return detail::index_of(value, ShaderType_values);
		}

		static constexpr CString nameOf(const ShaderType value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? ShaderType_names[i] : "";
		}


		// GL::Mode
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString Mode_names[] = {
			"Points",
			"Lines",
			"Line Loop",
			"Line Strip",
			"Triangles",
			"Triangle Strip",
			"Triangle Fan",
			"Fill",
		};

		static constexpr bool getEnum(const int32_t i, Mode & value)
		{
			return detail::value_at(i, value, Mode_values);
		}

		static constexpr int32_t indexOf(const Mode value)
		{
			return detail::index_of(value, Mode_values);
		}

		static constexpr CString nameOf(const Mode value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Mode_names[i] : "";
		}


		// GL::Equation
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Equation Equation_values[] = {
			Equation::Multiply,
			Equation::Add,
			Equation::Subtract,
			Equation::FuncAdd,
		};

		static constexpr CString Equation_names[] = {
			"Multiply",
			"Add",
			"Subtract",
			"Func Add",
		};

		static constexpr bool getEnum(const int32_t i, Equation & value)
		{
			return detail::value_at(i, value, Equation_values);
		}

		static constexpr int32_t indexOf(const Equation value)
		{
			return detail::index_of(value, Equation_values);
		}

		static constexpr CString nameOf(const Equation value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Equation_names[i] : "";
		}


		// GL::Comp
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr const Comp Comp_values[] = {
			Comp::Never,
			Comp::Less,
			Comp::Equal,
			Comp::LEqual,
			Comp::Greater,
			Comp::NotEqual,
			Comp::GEqual,
			Comp::Always,
		};

		static constexpr CString Comp_names[] = {
			"Never",
			"Less",
			"Equal",
			"Less or Equal",
			"Greater",
			"Not Equal",
			"Greater or Equal",
			"Always",
		};

		static constexpr bool getEnum(const int32_t i, Comp & value)
		{
			value = detail::cast_enum<Comp, int32_t>(i, Comp::Always);
			return true;
		}

		static constexpr int32_t indexOf(const Comp value)
		{
			return detail::cast_enum<int32_t, Comp>(value, Comp::Always);
		}

		static constexpr CString nameOf(const Comp value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Comp_names[i] : "";
		}

		
		// GL::Factor
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString Factor_names[] = {
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

		static constexpr bool getEnum(const int32_t i, Factor & value)
		{
			value = detail::cast_enum<Factor, int32_t>(i, Factor::SrcAlphaSaturate);
			return true;
		}

		static constexpr int32_t indexOf(const Factor value)
		{
			return detail::cast_enum<int32_t, Factor>(value, Factor::SrcAlphaSaturate);
		}

		static constexpr CString nameOf(const Factor value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Factor_names[i] : "";
		}


		// GL::Face
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString Face_names[] = {
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

		static constexpr bool getEnum(const int32_t i, Face & value)
		{
			value = detail::cast_enum<Face, int32_t>(i, Face::FrontAndBack);
			return true;
		}

		static constexpr int32_t indexOf(const Face value)
		{
			return detail::cast_enum<int32_t, Face>(value, Face::FrontAndBack);
		}

		static constexpr CString nameOf(const Face value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Face_names[i] : "";
		}


		// GL::Type
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString Type_names[] = {
			"Byte",
			"Unsigned Byte",
			"Short",
			"Unsigned Short",
			"Int",
			"Unsigned Int",
			"Float",
			"Half Float",
		};

		static constexpr bool getEnum(const int32_t i, Type & value)
		{
			return detail::value_at(i, value, Type_values);
		}

		static constexpr int32_t indexOf(const Type value)
		{
			return detail::index_of(value, Type_values);
		}

		static constexpr CString nameOf(const Type value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Type_names[i] : "";
		}


		// GL::Format
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString Format_names[] = {
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

		static constexpr bool getEnum(const int32_t i, Format & value)
		{
			return detail::value_at(i, value, Format_values);
		}

		static constexpr int32_t indexOf(const Format value)
		{
			return detail::index_of(value, Format_values);
		}

		static constexpr CString nameOf(const Format value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Format_names[i] : "";
		}


		// GL::FBO_Attachment
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr FBO_Attachment FBO_Attachment_values[] = {
			FBO_Attachment::AttachmentColorEncoding,
			FBO_Attachment::AttachmentComponentType,
			FBO_Attachment::AttachmentRedSize,
			FBO_Attachment::AttachmentGreenSize,
			FBO_Attachment::AttachmentBlueSize,
			FBO_Attachment::AttachmentAlphaSize,
			FBO_Attachment::AttachmentDepthSize,
			FBO_Attachment::AttachmentStencilSize,
			FBO_Attachment::FramebufferDefault,
			FBO_Attachment::FramebufferUndefined,
			FBO_Attachment::DepthStencilAttachment,
		};

		static constexpr CString FBO_Attachment_names[] = {
			"Attachment Color Encoding",
			"Attachment Component Type",
			"Attachment Red Size",
			"Attachment Green Size",
			"Attachment Blue Size",
			"Attachment Alpha Size",
			"Attachment Depth Size",
			"Attachment Stencil Size",
			"Framebuffer Default",
			"Framebuffer Undefined",
			"Depth Stencil Attachment",
		};

		static constexpr bool getEnum(const int32_t i, FBO_Attachment & value)
		{
			return detail::value_at(i, value, FBO_Attachment_values);
		}

		static constexpr int32_t indexOf(const FBO_Attachment value)
		{
			return detail::index_of(value, FBO_Attachment_values);
		}

		static constexpr CString nameOf(const FBO_Attachment value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? FBO_Attachment_names[i] : "";
		}


		// GL::TexParam
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			TexParam::CubeMap_Positive_X,
			TexParam::CubeMap_Negative_X,
			TexParam::CubeMap_Positive_Y,
			TexParam::CubeMap_Negative_Y,
			TexParam::CubeMap_Positive_Z,
			TexParam::CubeMap_Negative_Z,
		};

		static constexpr CString TexParam_names[] = {
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
			"CubeMap Positive X",
			"CubeMap Negative X",
			"CubeMap Positive Y",
			"CubeMap Negative Y",
			"CubeMap Positive Z",
			"CubeMap Negative Z",
		};

		static constexpr bool getEnum(const int32_t i, TexParam & value)
		{
			return detail::value_at(i, value, TexParam_values);
		}

		static constexpr int32_t indexOf(const TexParam value)
		{
			return detail::index_of(value, TexParam_values);
		}

		static constexpr CString nameOf(const TexParam value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? TexParam_names[i] : "";
		}


		// GL::Pack
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString Pack_names[] = {
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

		static constexpr bool getEnum(const int32_t i, Pack & value)
		{
			return detail::value_at(i, value, Pack_values);
		}

		static constexpr int32_t indexOf(const Pack value)
		{
			return detail::index_of(value, Pack_values);
		}

		static constexpr CString nameOf(const Pack value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Pack_names[i] : "";
		}


		// GL::Attachment
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Attachment Attachment_values[] = {
			Attachment::ColorAttachment0,
			Attachment::ColorAttachment1,
			Attachment::ColorAttachment2,
			Attachment::ColorAttachment3,
			Attachment::ColorAttachment4,
			Attachment::ColorAttachment5,
			Attachment::ColorAttachment6,
			Attachment::ColorAttachment7,
			Attachment::ColorAttachment8,
			Attachment::ColorAttachment9,
			Attachment::ColorAttachment10,
			Attachment::ColorAttachment11,
			Attachment::ColorAttachment12,
			Attachment::ColorAttachment13,
			Attachment::ColorAttachment14,
			Attachment::ColorAttachment15,
		};

		static constexpr CString Attachment_names[] = {
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

		static constexpr bool getEnum(const int32_t i, Attachment & value)
		{
			return detail::value_at(i, value, Attachment_values);
		}

		static constexpr int32_t indexOf(const Attachment value)
		{
			return detail::cast_enum<int32_t, Attachment>(value, Attachment::ColorAttachment15);
		}

		static constexpr CString nameOf(const Attachment value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Attachment_names[i] : "";
		}


		// GL::TexID
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString TexID_names[] = {
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

		static constexpr bool getEnum(const int32_t i, TexID & value)
		{
			return detail::value_at(i, value, TexID_values);
		}

		static constexpr int32_t indexOf(const TexID value)
		{
			return detail::cast_enum<int32_t, TexID>(value, TexID::Texture31);
		}

		static constexpr CString nameOf(const TexID value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? TexID_names[i] : "";
		}


		// GL::DrawBuffer
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static constexpr CString DrawBuffer_names[] = {
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

		static constexpr bool getEnum(const int32_t i, DrawBuffer & value)
		{
			return detail::value_at(i, value, DrawBuffer_values);
		}

		static constexpr int32_t indexOf(const DrawBuffer value)
		{
			return detail::cast_enum<int32_t, DrawBuffer>(value, DrawBuffer::DrawBuffer15);
		}

		static constexpr CString nameOf(const DrawBuffer value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? DrawBuffer_names[i] : "";
		}

		
		// GL::Mask
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Mask Mask_values[] = {
			ContextCoreProfileBit,
			ContextCompatProfileBit,
			ContextFlagDebugBit,
			DepthBufferBit,
			StencilBufferBit,
			ColorBufferBit,
		};

		static constexpr CString Mask_names[] = {
			"Context Core Profile Bit",
			"Context Compat Profile Bit",
			"Context Flag Debug Bit",
			"Depth Buffer Bit",
			"Stencil Buffer Bit",
			"Color Buffer Bit",
		};

		static constexpr bool getEnum(const int32_t i, Mask & value)
		{
			return detail::value_at(i, value, Mask_values);
		}

		static constexpr int32_t indexOf(const Mask value)
		{
			return detail::index_of(value, Mask_values);
		}

		static constexpr CString nameOf(const Mask value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Mask_names[i] : "";
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}

// Operators
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	// GL::Mask
	ML_GENERATE_BITMASK_OPERATORS(
		constexpr, GL::Mask, uint32_t
	);

	// GL::Attachment
	ML_GENERATE_ITERATOR_OPERATORS(
		constexpr, GL::Attachment, uint32_t,
		GL::Attachment::ColorAttachment0, GL::Attachment::ColorAttachment9
	);

	// GL::TexID
	ML_GENERATE_ITERATOR_OPERATORS(
		constexpr, GL::TexID, uint32_t, 
		GL::TexID::Texture0, GL::TexID::Texture31
	);
}