// https://github.com/BalazsJako/ImGuiColorTextEdit/blob/master/TextEditor.h
// Modified - Melody Gurman - 2019 - https://github.com/Gurman8r

#pragma once

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <regex>
#include <imgui/imgui.h>

namespace ImGui
{
	class IMGUI_API TextEditor
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		enum class PaletteIndex
		{
			Default,
			Keyword,
			Number,
			String,
			CharLiteral,
			Punctuation,
			Preprocessor,
			Identifier,
			KnownIdentifier,
			PreprocIdentifier,
			Comment,
			MultiLineComment,
			Background,
			Cursor,
			Selection,
			ErrorMarker,
			Breakpoint,
			LineNumber,
			CurrentLineFill,
			CurrentLineFillInactive,
			CurrentLineEdge,
			Max
		};

		enum class SelectionMode
		{
			Normal,
			Word,
			Line
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct IMGUI_API Breakpoint
		{
			int32_t mLine;
			bool mEnabled;
			std::string mCondition;

			Breakpoint()
				: mLine(-1)
				, mEnabled(false)
			{
			}
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct IMGUI_API Coordinates
		{
			// Represents a character coordinate from the user's point of view,
			// i. e. consider an uniform grid (assuming fixed-width font) on the
			// screen as it is rendered, and each cell has its own coordinate, starting from 0.
			// Tabs are counted as [1..mTabSize] count empty spaces, depending on
			// how many space is necessary to reach the next tab stop.
			// For example, coordinate (1, 5) represents the character 'B' in a line "\tABC", when mTabSize = 4,
			// because it is rendered as "    ABC" on the screen.

			int32_t mLine, mColumn;
			Coordinates() : mLine(0), mColumn(0) {}
			Coordinates(int32_t aLine, int32_t aColumn) : mLine(aLine), mColumn(aColumn)
			{
				assert(aLine >= 0);
				assert(aColumn >= 0);
			}
			static inline Coordinates Invalid() { static Coordinates invalid(-1, -1); return invalid; }

			inline bool operator ==(const Coordinates & o) const
			{
				return
					mLine == o.mLine &&
					mColumn == o.mColumn;
			}

			inline bool operator !=(const Coordinates & o) const
			{
				return
					mLine != o.mLine ||
					mColumn != o.mColumn;
			}

			inline bool operator <(const Coordinates & o) const
			{
				return (mLine != o.mLine)
					? mLine < o.mLine
					: mColumn < o.mColumn;
			}

			inline bool operator >(const Coordinates & o) const
			{
				return (mLine != o.mLine)
					? mLine > o.mLine
					: mColumn > o.mColumn;
			}

			inline bool operator <=(const Coordinates & o) const
			{
				return (mLine != o.mLine)
					? mLine < o.mLine
					: mColumn <= o.mColumn;
			}

			inline bool operator >=(const Coordinates & o) const
			{
				return (mLine != o.mLine)
					? mLine > o.mLine
					: mColumn >= o.mColumn;
			}
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct IMGUI_API Identifier
		{
			Coordinates mLocation;
			std::string mDeclaration;
		};

		typedef std::string String;
		typedef std::unordered_map<std::string, Identifier> Identifiers;
		typedef std::unordered_set<std::string> Keywords;
		typedef std::map<int32_t, std::string> ErrorMarkers;
		typedef std::unordered_set<int32_t> Breakpoints;
		typedef std::array<ImU32, (unsigned)PaletteIndex::Max> Palette;
		typedef uint8_t Char;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct IMGUI_API Glyph
		{
			Char mChar;
			PaletteIndex mColorIndex = PaletteIndex::Default;
			bool mComment : 1;
			bool mMultiLineComment : 1;
			bool mPreprocessor : 1;

			Glyph(Char aChar, PaletteIndex aColorIndex) : mChar(aChar), mColorIndex(aColorIndex),
				mComment(false), mMultiLineComment(false), mPreprocessor(false)
			{
			}
		};

		typedef std::vector<Glyph> Line;
		typedef std::vector<Line> Lines;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct IMGUI_API LanguageDefinition
		{
			typedef std::pair<std::string, PaletteIndex> TokenRegexString;
			typedef std::vector<TokenRegexString> TokenRegexStrings;
			typedef bool(*TokenizeCallback)(const char * in_begin, const char * in_end, const char *& out_begin, const char *& out_end, PaletteIndex & paletteIndex);

			std::string			mName;
			Keywords			mKeywords;
			Identifiers			mIdentifiers;
			Identifiers			mPreprocIdentifiers;
			std::string			mCommentStart; 
			std::string			mCommentEnd;
			std::string			mSingleLineComment;
			char				mPreprocChar;
			bool				mAutoIndentation;
			TokenizeCallback	mTokenize;
			TokenRegexStrings	mTokenRegexStrings;
			bool				mCaseSensitive;

			LanguageDefinition()
				: mPreprocChar('#')
				, mAutoIndentation(true)
				, mTokenize(nullptr)
				, mCaseSensitive(true)
			{
			}

			static const LanguageDefinition & CPlusPlus();
			static const LanguageDefinition & HLSL();
			static const LanguageDefinition & GLSL();
			static const LanguageDefinition & C();
			static const LanguageDefinition & SQL();
			static const LanguageDefinition & AngelScript();
			static const LanguageDefinition & Lua();
			static const LanguageDefinition & Python();

			inline static const LanguageDefinition * get(const int value)
			{
				switch (value)
				{
				case 0: return &CPlusPlus();
				case 1: return &HLSL();
				case 2: return &GLSL();
				case 3: return &C();
				case 4: return &SQL();
				case 5: return &AngelScript();
				case 6: return &Lua();
				case 7: return &Python();
				default: return nullptr;
				}
			}
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		TextEditor();
		TextEditor(const std::string & text);
		~TextEditor();

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		void SetLanguageDefinition(const LanguageDefinition & aLanguageDef);
		const LanguageDefinition & GetLanguageDefinition() const { return mLanguageDefinition; }

		void SetPalette(const Palette & aValue);

		void SetErrorMarkers(const ErrorMarkers & aMarkers) { mErrorMarkers = aMarkers; }
		void SetBreakpoints(const Breakpoints & aMarkers) { mBreakpoints = aMarkers; }

		void Render(const char* aTitle, const ImVec2 & aSize = ImVec2(), bool aBorder = false);
		void SetText(const std::string & aText);
		std::string GetText() const;

		void SetTextLines(const std::vector<std::string> & aLines);
		std::vector<std::string> GetTextLines() const;

		std::string GetSelectedText() const;
		std::string GetCurrentLineText()const;

		void SetReadOnly(bool aValue);
		void SetColorizerEnable(bool aValue);
		void SetCursorPosition(const Coordinates & aPosition);
		void SetTabSize(int32_t aValue);

		void InsertText(const std::string & aValue);
		void InsertText(const char* aValue);

		void MoveUp(int32_t aAmount = 1, bool aSelect = false);
		void MoveDown(int32_t aAmount = 1, bool aSelect = false);
		void MoveLeft(int32_t aAmount = 1, bool aSelect = false, bool aWordMode = false);
		void MoveRight(int32_t aAmount = 1, bool aSelect = false, bool aWordMode = false);
		void MoveTop(bool aSelect = false);
		void MoveBottom(bool aSelect = false);
		void MoveHome(bool aSelect = false);
		void MoveEnd(bool aSelect = false);

		void SetSelectionStart(const Coordinates & aPosition);
		void SetSelectionEnd(const Coordinates & aPosition);
		void SetSelection(const Coordinates & aStart, const Coordinates & aEnd, SelectionMode aMode = SelectionMode::Normal);
		void SelectWordUnderCursor();
		void SelectAll();
		bool HasSelection() const;

		void Copy();
		void Cut();
		void Paste();
		void Delete();

		bool CanUndo() const;
		bool CanRedo() const;
		void Undo(int32_t aSteps = 1);
		void Redo(int32_t aSteps = 1);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static const Palette & GetDarkPalette();
		static const Palette & GetLightPalette();
		static const Palette & GetRetroBluePalette();

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline auto GetPalette() const -> const Palette & { return mPaletteBase; }
		inline auto GetTotalLines() const -> int32_t { return (int32_t)mLines.size(); }
		inline auto	GetTabSize() const -> int32_t { return mTabSize; }
		inline auto GetCursorPosition() const -> Coordinates { return GetActualCursorCoordinates(); }
		
		inline bool IsOverwrite() const { return mOverwrite; }
		inline bool IsReadOnly() const { return mReadOnly; }
		inline bool IsTextChanged() const { return mTextChanged; }
		inline bool IsCursorPositionChanged() const { return mCursorPositionChanged; }
		inline bool IsColorizerEnabled() const { return mColorizerEnabled; }
		inline bool IsHandleMouseInputsEnabled() const { return mHandleKeyboardInputs; }
		inline bool IsHandleKeyboardInputsEnabled() const { return mHandleKeyboardInputs; }
		inline bool IsImGuiChildIgnored() const { return mIgnoreImGuiChild; }
		inline bool IsShowingWhitespaces() const { return mShowWhitespaces; }
		
		inline void SetHandleKeyboardInputs(bool aValue) { mHandleKeyboardInputs = aValue; }
		inline void SetHandleMouseInputs(bool aValue) { mHandleMouseInputs = aValue; }
		inline void SetImGuiChildIgnored(bool aValue) { mIgnoreImGuiChild = aValue; }
		inline void SetShowWhitespaces(bool aValue) { mShowWhitespaces = aValue; }

	private:
		typedef std::vector<std::pair<std::regex, PaletteIndex>> RegexList;

		struct EditorState
		{
			Coordinates mSelectionStart;
			Coordinates mSelectionEnd;
			Coordinates mCursorPosition;
		};

		class UndoRecord
		{
		public:
			UndoRecord() {}
			~UndoRecord() {}

			UndoRecord(
				const std::string & aAdded,
				const TextEditor::Coordinates aAddedStart,
				const TextEditor::Coordinates aAddedEnd,

				const std::string & aRemoved,
				const TextEditor::Coordinates aRemovedStart,
				const TextEditor::Coordinates aRemovedEnd,

				TextEditor::EditorState& aBefore,
				TextEditor::EditorState& aAfter);

			void Undo(TextEditor* aEditor);
			void Redo(TextEditor* aEditor);

			std::string mAdded;
			Coordinates mAddedStart;
			Coordinates mAddedEnd;

			std::string mRemoved;
			Coordinates mRemovedStart;
			Coordinates mRemovedEnd;

			EditorState mBefore;
			EditorState mAfter;
		};

		typedef std::vector<UndoRecord> UndoBuffer;

		void ProcessInputs();
		void Colorize(int32_t aFromLine = 0, int32_t aCount = -1);
		void ColorizeRange(int32_t aFromLine = 0, int32_t aToLine = 0);
		void ColorizeInternal();
		float TextDistanceToLineStart(const Coordinates & aFrom) const;
		void EnsureCursorVisible();
		int32_t GetPageSize() const;
		std::string GetText(const Coordinates & aStart, const Coordinates & aEnd) const;
		Coordinates GetActualCursorCoordinates() const;
		Coordinates SanitizeCoordinates(const Coordinates & aValue) const;
		void Advance(Coordinates & aCoordinates) const;
		void DeleteRange(const Coordinates & aStart, const Coordinates & aEnd);
		int32_t InsertTextAt(Coordinates & aWhere, const char* aValue);
		void AddUndo(UndoRecord& aValue);
		Coordinates ScreenPosToCoordinates(const ImVec2 & aPosition) const;
		Coordinates FindWordStart(const Coordinates & aFrom) const;
		Coordinates FindWordEnd(const Coordinates & aFrom) const;
		Coordinates FindNextWord(const Coordinates & aFrom) const;
		int32_t GetCharacterIndex(const Coordinates & aCoordinates) const;
		int32_t GetCharacterColumn(int32_t aLine, int32_t aIndex) const;
		int32_t GetLineCharacterCount(int32_t aLine) const;
		int32_t GetLineMaxColumn(int32_t aLine) const;
		bool IsOnWordBoundary(const Coordinates & aAt) const;
		void RemoveLine(int32_t aStart, int32_t aEnd);
		void RemoveLine(int32_t aIndex);
		Line& InsertLine(int32_t aIndex);
		void EnterCharacter(ImWchar aChar, bool aShift);
		void Backspace();
		void DeleteSelection();
		std::string GetWordUnderCursor() const;
		std::string GetWordAt(const Coordinates & aCoords) const;
		ImU32 GetGlyphColor(const Glyph& aGlyph) const;

		void HandleKeyboardInputs();
		void HandleMouseInputs();
		void Render();

		float mLineSpacing;
		Lines mLines;
		EditorState mState;
		UndoBuffer mUndoBuffer;
		int32_t mUndoIndex;

		int32_t mTabSize;
		bool mOverwrite;
		bool mReadOnly;
		bool mWithinRender;
		bool mScrollToCursor;
		bool mScrollToTop;
		bool mTextChanged;
		bool mColorizerEnabled;
		float mTextStart;                   // position (in pixels) where a code line starts relative to the left of the TextEditor.
		int32_t  mLeftMargin;
		bool mCursorPositionChanged;
		int32_t mColorRangeMin, mColorRangeMax;
		SelectionMode mSelectionMode;
		bool mHandleKeyboardInputs;
		bool mHandleMouseInputs;
		bool mIgnoreImGuiChild;
		bool mShowWhitespaces;

		Palette mPaletteBase;
		Palette mPalette;
		LanguageDefinition mLanguageDefinition;
		RegexList mRegexList;

		bool mCheckComments;
		Breakpoints mBreakpoints;
		ErrorMarkers mErrorMarkers;
		ImVec2 mCharAdvance;
		Coordinates mInteractiveStart, mInteractiveEnd;
		std::string mLineBuffer;
		uint64_t mStartTime;

		float mLastClick;
	};
}