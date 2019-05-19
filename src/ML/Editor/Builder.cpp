#include <ML/Editor/Builder.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/GUI.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Graphics/ShaderParser.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Uni.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_VERT_EXAMPLE \
"#shader vertex\n" \
"#version 410 core\n" \
"\n" \
"/* * * * * * * * * * * * * * * * * * * * */\n" \
"\n" \
"layout(location = 0) in vec3 a_Position;\n" \
"layout(location = 1) in vec4 a_Normal;\n" \
"layout(location = 2) in vec2 a_Texcoord;\n" \
"\n" \
"out VertexData\n" \
"{\n" \
"	vec3 Position;\n" \
"	vec4 Normal;\n" \
"	vec2 Texcoord;\n" \
"} Out;\n" \
"\n" \
"struct Vert_Uniforms\n" \
"{\n" \
"	mat4 proj;\n" \
"	mat4 view;\n" \
"	mat4 model;\n" \
"};\n" \
"\n" \
"uniform Vert_Uniforms Vert;\n" \
"\n" \
"/* * * * * * * * * * * * * * * * * * * * */\n" \
"\n" \
""

#define ML_FRAG_EXAMPLE \
"#shader fragment\n" \
"#version 410 core\n" \
"\n" \
"/* * * * * * * * * * * * * * * * * * * * */\n" \
"\n" \
"in VertexData\n" \
"{\n" \
"	vec3 Position;\n" \
"	vec4 Normal;\n" \
"	vec2 Texcoord;\n" \
"} In;\n" \
"\n" \
"out vec4 gl_Color;\n" \
"\n" \
"struct Frag_Uniforms\n" \
"{\n" \
"	vec4 mainCol;\n" \
"	sampler2D mainTex;\n" \
"};\n" \
"\n" \
"uniform Frag_Uniforms Frag;\n" \
"\n" \
"/* * * * * * * * * * * * * * * * * * * * */\n" \
""

#define ML_MAIN_EXAMPLE \
"/* * * * * * * * * * * * * * * * * * * * */\n" \
"\n" \
"#include \"Vertex\"\n" \
"\n" \
"void main()\n" \
"{\n" \
"	Out.Position = a_Position;\n" \
"	Out.Normal   = a_Normal;\n" \
"	Out.Texcoord = a_Texcoord;\n" \
"	\n" \
"	mat4 MVP = (Vert.proj * Vert.view * Vert.model);\n" \
"	\n" \
"	gl_Position = MVP * vec4(Out.Position, 1.0);\n" \
"}\n" \
"\n" \
"/* * * * * * * * * * * * * * * * * * * * */\n" \
"\n" \
"#include \"Fragment\"\n" \
"\n" \
"void main()\n" \
"{\n" \
"	gl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);\n" \
"}\n" \
"\n" \
"/* * * * * * * * * * * * * * * * * * * * */\n" \
""

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Builder::Builder()
		: BaseWidget("Builder")
		, m_shader	(ML_Resources.shaders.load(ML_TEST_SHADER))
	{
		m_files.push_back(new BuildFile("Main", ML_MAIN_EXAMPLE));
		m_files.push_back(new BuildFile("Vertex", ML_VERT_EXAMPLE));
		m_files.push_back(new BuildFile("Fragment", ML_FRAG_EXAMPLE));
	}

	Builder::~Builder()
	{
		for (auto it : m_files)
		{
			delete it;
		}
		m_files.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::onEvent(const IEvent * value)
	{
	}

	bool Builder::drawGui(const GuiEvent * ev, bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			// Menu Bar
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenuBar())
			{
				// New
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::MenuItem("New"))
				{
					ImGui::OpenPopup("New File");
				}
				if (ImGui::BeginPopupModal("New File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					static char name[32] = "New File";

					auto closePopup = [&]()
					{
						std::strcpy(name, "New File");
						ImGui::CloseCurrentPopup();
					};

					ImGui::InputText("Name", name, IM_ARRAYSIZE(name),
						ImGuiInputTextFlags_EnterReturnsTrue
					);

					if (ImGui::Button("Submit"))
					{
						auto addNewFile = [&]()
						{
							if (!String(name))
							{
								Debug::logError("Name cannot be empty");
								return;
							}
							for (auto file : m_files)
							{
								if (file->name == name)
								{
									Debug::logError("File with name \'{0}\' already exists", name);
									return;
								}
							}
							m_files.push_back(new BuildFile(name, String()));
						};
						addNewFile();
						closePopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						closePopup();
					}

					ImGui::EndPopup();
				}

				// Compile
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::MenuItem("Compile"))
				{
					for (auto & file : m_files)
					{
						file->dirty = false;
					}

					struct BuilderParser
					{
						inline static String parseIncludes(const List<BuildFile *> & files, const String & src)
						{
							SStream out;
							SStream ss(src);
							String line;
							while (std::getline(ss, line))
							{
								if (line.find("#include") != String::npos)
								{
									bool found = false;
									String name;
									if (ShaderParser::parseWrapped(line, '\"', '\"', name))
									{
										for (auto f : files)
										{
											if (f->name == name)
											{
												out << parseIncludes(files, f->text);
												found = true;
												break;
											}
										}
									}
									if (!found)
									{
										out << line << endl;
									}
								}
								else
								{
									out << line << endl;
								}
							}

							return out.str();
						}
					};

					String source = BuilderParser::parseIncludes(
						m_files, 
						m_files.front()->text
					);

					if (m_shader && m_shader->loadFromMemory(source))
					{
						Debug::log("Compiled Shader: {0}", ML_TEST_SHADER);
					}
					else
					{
						Debug::logError("Failed Compiling Shader:\n{0}", ML_TEST_SHADER);
					}
				}

				ImGui::EndMenuBar();
			}

			// File Tabs Bar
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			ImGui::BeginGroup();
			ImGui::BeginChild("Shader Content Tabs");
			if (ImGui::BeginTabBar("Shader Content Tabs", ImGuiTabBarFlags_Reorderable))
			{
				/* * * * * * * * * * * * * * * * * * * * */

				std::vector<BuildMap::iterator> toRemove;

				/* * * * * * * * * * * * * * * * * * * * */

				for (auto it = m_files.begin(); it != m_files.end(); it++)
				{
					BuildFile *	file = (*it);
				
					const size_t i = (it - m_files.begin());

					bool * t_open = (i > 0
						? (&file->open)
						: (NULL)
					);

					// File Tab
					if (ImGui::BeginTabItem(
						String("[" + std::to_string(i) + "] " + file->name).c_str(),
						t_open,
						file->dirty ? ImGuiTabItemFlags_UnsavedDocument : 0
					))
					{	// Input Text Content Area
						if (ImGui::BeginChild(
							"InputTextContentArea",
							{ 0, 0 },
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{	/* * * * * * * * * * * * * * * * * * * * */

							// Disallow editing Main's name
							if (i > 0)
							{
								char buf[32];
								std::strcpy(buf, file->name.c_str());
								if (ImGui::InputText(
									"Name",
									buf,
									Document::NameSize,
									ImGuiInputTextFlags_EnterReturnsTrue
								))
								{
									file->name = buf;
								}
							}

							if (ImGui::InputTextMultiline(
								String("##File" + file->name + "##Text").c_str(),
								file->text,
								BuildFile::MaxSize,
								{ -1.f, -1.f },
								ImGuiInputTextFlags_AllowTabInput
							))
							{
								file->dirty = true;
							}

							/* * * * * * * * * * * * * * * * * * * * */

							ImGui::EndChild();
						}
						ImGui::EndTabItem();
					}

					if (!file->open)
					{
						toRemove.push_back(it);
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */

				for (auto it : toRemove)
				{
					delete (*it);
					m_files.erase(it);
				}

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::EndGroup();

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}