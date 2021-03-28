#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <string_view>
#include <string>

#include "CRandom16.hpp"
//#include "Runtime/Graphics/CLineRenderer.hpp"
//#include "Runtime/RetroTypes.hpp"
//#include "Runtime/World/CActor.hpp"

//#include <zeus/CVector3f.hpp>

constexpr s32 skMazeCols = 9;
constexpr s32 skMazeRows = 7;
constexpr s32 skEnterCol = 4;
constexpr s32 skEnterRow = 4;
constexpr s32 skTargetCol = 5;
constexpr s32 skTargetRow = 3;

enum class ESide {
  Invalid = -1,
  Top = 0,
  Right = 1,
  Bottom = 2,
  Left = 3,
};

struct SMazeCell {
  bool x0_24_openTop : 1 = false;
  bool x0_25_openRight : 1 = false;
  bool x0_26_openBottom : 1 = false;
  bool x0_27_openLeft : 1 = false;
  bool x0_28_gateTop : 1 = false;
  bool x0_29_gateRight : 1 = false;
  bool x0_30_gateBottom : 1 = false;
  bool x0_31_gateLeft : 1 = false;
  bool x1_24_puddle : 1 = false;
  bool x1_25_onPath : 1 = false;
  bool x1_26_checked : 1 = false;

  [[nodiscard]] constexpr bool IsOpen() const {
    return x0_24_openTop || x0_25_openRight || x0_26_openBottom || x0_27_openLeft;
  }
};

class CMazeState {
  CRandom16 x0_rand{0};
  std::array<SMazeCell, skMazeRows * skMazeCols> x4_cells{};
  s32 x84_enterCol;
  s32 x88_enterRow;
  s32 x8c_targetCol;
  s32 x90_targetRow;
  bool x94_24_initialized : 1 = false;

  std::vector<s32> m_path;
//  CLineRenderer m_renderer = {CLineRenderer::EPrimitiveMode::LineStrip, skMazeRows * skMazeCols, {}, true};

public:
  CMazeState(s32 enterCol, s32 enterRow, s32 targetCol, s32 targetRow)
  : x84_enterCol(enterCol), x88_enterRow(enterRow), x8c_targetCol(targetCol), x90_targetRow(targetRow) {}
  void Reset(s32 seed);
  void Initialize();
  void GenerateObstacles();
  std::string to_str();

//  void DebugRender();

  [[nodiscard]] SMazeCell& GetCell(u32 col, u32 row) {
#ifndef NDEBUG
    assert(col < skMazeCols);
    assert(row < skMazeRows);
#endif
    return x4_cells[col + row * skMazeCols];
  }
  [[nodiscard]] SMazeCell& GetCell(u32 idx) {
#ifndef NDEBUG
    assert(idx < x4_cells.size());
#endif
    return x4_cells[idx];
  }
};