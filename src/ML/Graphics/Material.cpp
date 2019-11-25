#include <ML/Graphics/Material.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Input.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material::Material()
		: m_uniforms {}
	{
	}

	Material::Material(Material const & copy)
		: m_uniforms {}
	{
		for (auto const & u : copy)
		{
			if (u && (this->find(u->name()) == this->end()))
			{
				this->insert(u->clone());
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::dispose()
	{
		for (auto & u : m_uniforms)
		{
			if (u) { delete u; }
		}
		m_uniforms.clear();
		return m_uniforms.empty();
	}

	bool Material::loadFromFile(String const & filename)
	{
		return loadFromFile(filename, nullptr);
	}

	bool Material::loadFromFile(String const & filename, std::map<String, Texture *> const * textures)
	{
		// Load uniforms from file
		if (std::ifstream file { filename })
		{
			auto pop_front = ([](std::vector<String> & toks) {
				// Erase and return front
				if (toks.empty()) return String{};
				const String temp{ toks.front() };
				toks.erase(toks.begin());
				return temp;
			});


			// Parse Lines
			String line;
			while (std::getline(file, line))
			{
				// Skip if empty or comment
				/* * * * * * * * * * * * * * * * * * * * */
				if (line.empty() || line.trim().front() == '#')
					continue;

				// Parse tokens from line
				/* * * * * * * * * * * * * * * * * * * * */
				std::vector<String> tokens{ ([](String line)
				{
					std::vector<String> toks;
					if (!line) return toks;
					line.trim()
						.replace_all("\t", " ")
						.replace_all(",", "");
					size_t idx = 0;
					while ((idx = line.find(" ")) != String::npos)
					{
						String temp = line.substr(0, idx);
						if (temp) toks.push_back(temp);
						line.erase(0, idx + 1);
					}
					if (line) toks.push_back(line);
					return toks;
				})(line) };

				// Parse uniform from tokens
				/* * * * * * * * * * * * * * * * * * * * */
				if (!tokens.empty() && (pop_front(tokens) == "uniform"))
				{
					// Uniform Type
					/* * * * * * * * * * * * * * * * * * * * */
					const hash_t u_type{ ([](C_String type)
					{
						const hash_t imax{ (hash_t)Uniform::Type::ID_MAX };
						if (!type) { return imax; }
						for (hash_t i = 0; i < imax; i++)
						{
							if (std::strcmp(type, Uniform::Type_names[i]) == 0)
							{
								return i;
							}
						}
						return imax;
					})(pop_front(tokens).c_str()) };

					// Uniform Name
					/* * * * * * * * * * * * * * * * * * * * */
					const String u_name{ pop_front(tokens) };

					// Uniform Data
					/* * * * * * * * * * * * * * * * * * * * */
					SStream u_data{ ([](std::vector<String> & toks)
					{
						SStream out;
						if ((toks.size() > 2 && toks.front() == "{" && toks.back() == "}"))
						{
							toks.erase(toks.begin());
							String temp;
							while (!toks.empty() && ((temp = toks.front()) != "}"))
							{
								out << temp << ' ';
								toks.erase(toks.begin());
							}
						}
						return out;
					})(tokens) };

					// Generate Uniform
					/* * * * * * * * * * * * * * * * * * * * */
					if (Uniform * u{ ([](hash_t type, String const & name, SStream & ss, auto const * textures)
					{
						Uniform * u{ nullptr };
						if ((type == -1) || name.empty() || ss.str().empty()) { return u; }
						switch (type)
						{
						case Uniform::category_of<bool>():
							return u = new uni_bool{ name, input<bool>()(ss) };

						case Uniform::category_of<int32_t>():
							return u = new uni_int{ name, input<int32_t>()(ss) };

						case Uniform::category_of<float_t>():
							return u = new uni_float{ name, input<float_t>()(ss) };

						case Uniform::category_of<vec2f>():
							return u = new uni_vec2{ name, input<vec2>()(ss) };

						case Uniform::category_of<vec3f>():
							return u = new uni_vec3{ name, input<vec3>()(ss) };

						case Uniform::category_of<vec4f>():
							return u = new uni_vec4{ name, input<vec4>()(ss) };

						case Uniform::category_of<Color>():
							return u = new uni_color{ name, input<vec4>()(ss) };

						case Uniform::category_of<mat2f>():
							return u = new uni_mat2{ name, input<mat2>()(ss) };

						case Uniform::category_of<mat3f>():
							return u = new uni_mat3{ name, input<mat3>()(ss) };

						case Uniform::category_of<mat4f>():
							return u = new uni_mat4{ name, input<mat4>()(ss) };

						case Uniform::category_of<Texture>():
							if (textures)
							{
								auto it{ textures->find(String{ ss.str() }.trim()) };
								return ((it != textures->end())
									? u = new uni_sampler{ name, it->second }
									: u = new uni_sampler{ name, nullptr }
								);
							}
						}
						return (u = nullptr);
					})(u_type, u_name, u_data, textures)})
					{
						this->insert(u);
					}
				}
			}


			// Sort
			std::sort(this->begin(), this->end(), [&](auto a, auto b) {
				return a->name() < b->name();
			});

			file.close();
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}