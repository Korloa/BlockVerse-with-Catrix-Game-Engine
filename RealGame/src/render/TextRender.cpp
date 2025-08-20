#include "render/text/TextRender.h"
#include "render/Shader.h"
//#include <utf8.h> ��ʱ��������������ַ��������ֽڶ����
//���鷳
//#include <stdexcept>
//֮��Ҫʵ�ֶ�̬��������

TextRender::TextRender():ft(nullptr),face(nullptr),textShader(nullptr),vao(0),vbo(0){}
TextRender::~TextRender() {
	for (auto& c : characters)
		glDeleteTextures(1, &c.second.textureID);
	// map ,unorderedmap set unorderedset list deque���ڴ��д洢����������
	// vector,array,t[],string���������洢��
	characters.clear();
	if (face)
		FT_Done_Face(face);
	if (ft)
		FT_Done_FreeType(ft);
	if (textShader)
		delete textShader;
	if (vao)
		glDeleteVertexArrays(1, &vao);
	if (vbo)
		glDeleteBuffers(1, &vbo);
}

bool TextRender::initialize(const std::string& fontPath, unsigned int fontSize) {
	//const ��ֹ�޸� &���Է�ֹ���� ������� const A& > A
	// ft��ɹ�����0�����ɹ����ط�0
	if (FT_Init_FreeType(&ft)) {
		console.error("Failed to initialize FreeType textrender.");
	}
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)){
		FT_Done_FreeType(ft);
		console.error("Failed to load the font.");
	}
	// what about char size??
	// 0 �����Զ�����
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	//gl_unpack_alignment
	//�Ⲣ����Ӱ��Ч�ʣ���ΪOpenGL�ǰ����ȡ��
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//ASCII
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//? һ��״̬�����󶨺���������ö�������texture
		glTexImage2D(
			GL_TEXTURE_2D,				//��������
			0,							//������
			GL_RED,						//���ݵ�ռ��ͨ��
			face->glyph->bitmap.width,	//������
			face->glyph->bitmap.rows,	//����߶�
			0,							//�Ƿ�����߿� 0Ϊû�� �ִ�OpenGL����Ϊ0
			GL_RED,						//GPU�洢����
			GL_UNSIGNED_BYTE,			//λͼ���ݸ�ʽ
			face->glyph->bitmap.buffer	//λͼ��������
		);
		//�����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//�����������곬��ʱҪ��δ���
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//������Сʱ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//�����Ŵ�ʱ

		characters[c] = {
			texture,
			glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
	}
	//���
	glBindTexture(GL_TEXTURE_2D, 0);

	textShader = new Shader("text_vertex.glsl", "text_fragment.glsl");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}