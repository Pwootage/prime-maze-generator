#include "CScriptMazeNode.hpp"

//#include "Runtime/CStateManager.hpp"
//#include "Runtime/Character/CModelData.hpp"
//#include "Runtime/GameGlobalObjects.hpp"
//#include "Runtime/World/CActorParameters.hpp"

//#include "TCastTo.hpp" // Generated file, do not modify include path


std::array<s32, 300> sMazeSeeds;

//std::array<zeus::CVector3f, skMazeRows * skMazeCols> sDebugCellPos;

void CMazeState::Reset(s32 seed) {
  x0_rand.SetSeed(seed);
  x94_24_initialized = false;
  x4_cells.fill({});

  std::array<ESide, 4> sides{};
  s32 cellIdx = 0;
  for (u32 i = skMazeCols * skMazeRows - 1; i != 0;) {
    u32 acc = 0;
    if (cellIdx - skMazeCols > 0 && !GetCell(cellIdx - skMazeCols).IsOpen()) {
      sides[acc++] = ESide::Top;
    }
    if (cellIdx < x4_cells.size() - 2 && (cellIdx + 1) % skMazeCols != 0 && !GetCell(cellIdx + 1).IsOpen()) {
      sides[acc++] = ESide::Right;
    }
    if (cellIdx + skMazeCols < x4_cells.size() && !GetCell(cellIdx + skMazeCols).IsOpen()) {
      sides[acc++] = ESide::Bottom;
    }
    if (cellIdx > 0 && cellIdx % skMazeCols != 0 && !GetCell(cellIdx - 1).IsOpen()) {
      sides[acc++] = ESide::Left;
    }

    if (acc == 0) {
      do {
        cellIdx++;
        if (cellIdx > x4_cells.size() - 1) {
          cellIdx = 0;
        }
      } while (!GetCell(cellIdx).IsOpen());
      continue;
    }

    i--;
    ESide side = sides[x0_rand.Next() % acc];
    if (side == ESide::Bottom) {
      GetCell(cellIdx).x0_26_openBottom = true;
      GetCell(cellIdx + skMazeCols).x0_24_openTop = true;
      cellIdx += skMazeCols;
    } else if (side == ESide::Top) {
      GetCell(cellIdx).x0_24_openTop = true;
      GetCell(cellIdx - skMazeCols).x0_26_openBottom = true;
      cellIdx -= skMazeCols;
    } else if (side == ESide::Right) {
      GetCell(cellIdx).x0_25_openRight = true;
      GetCell(cellIdx + 1).x0_27_openLeft = true;
      cellIdx++;
    } else if (side == ESide::Left) {
      GetCell(cellIdx).x0_27_openLeft = true;
      GetCell(cellIdx - 1).x0_25_openRight = true;
      cellIdx--;
    }
  }
}

void CMazeState::Initialize() {
  std::array<s32, skMazeRows * skMazeCols> path{};
  path[0] = x84_enterCol + x88_enterRow * skMazeCols;
  GetCell(path[0]).x1_26_checked = true;
  s32 pathLength = 1;
  while (path[0] != x8c_targetCol + x90_targetRow * skMazeCols) {
    if (GetCell(path[0]).x0_24_openTop) {
      if (!GetCell(path[0] - skMazeCols).x1_26_checked) {
        path[pathLength] = path[0] - skMazeCols;
        pathLength++;
      }
    }
    if (GetCell(path[0]).x0_25_openRight) {
      if (!GetCell(path[0] + 1).x1_26_checked) {
        path[pathLength] = path[0] + 1;
        pathLength++;
      }
    }
    if (GetCell(path[0]).x0_26_openBottom) {
      if (!GetCell(path[0] + skMazeCols).x1_26_checked) {
        path[pathLength] = path[0] + skMazeCols;
        pathLength++;
      }
    }
    if (GetCell(path[0]).x0_27_openLeft) {
      if (!GetCell(path[0] - 1).x1_26_checked) {
        path[pathLength] = path[0] - 1;
        pathLength++;
      }
    }
    if (path[0] == path[pathLength - 1]) {
      pathLength--;
    }
    path[0] = path[pathLength - 1];
    GetCell(path[0]).x1_26_checked = true;
  }
  s32 *idx = &path[pathLength];
  while (pathLength != 0) {
    pathLength--;
    idx--;
    auto &cell = GetCell(*idx);
    if (cell.x1_26_checked) {
      cell.x1_25_onPath = true;
      if (pathLength > 0) {
        m_path.push_back(*idx);
      }
    }
  }
  x94_24_initialized = true;
}

void CMazeState::GenerateObstacles() {
  if (!x94_24_initialized) {
    Initialize();
  }

  auto GetRandom = [this](s32 offset) {
    s32 tmp = x0_rand.Next();
    return tmp + ((tmp / 5) * -5) + offset;
  };
  s32 gate1Idx = GetRandom(9);
  s32 gate2Idx = GetRandom(21);
  s32 gate3Idx = GetRandom(33);
  s32 puddle1Idx = GetRandom(13);
  s32 puddle2Idx = GetRandom(29);

  ESide side = ESide::Invalid;
  s32 idx = 0;

  s32 prevCol = x84_enterCol;
  s32 prevRow = x88_enterRow;
  s32 col = x84_enterCol;
  s32 row = x88_enterRow;

  while (col != x8c_targetCol || row != x90_targetRow) {
    if (idx == gate1Idx || idx == gate2Idx || idx == gate3Idx) {
      if (side == ESide::Bottom) {
        GetCell(col, row).x0_28_gateTop = true;
        GetCell(prevCol, prevRow).x0_30_gateBottom = true;
      } else if (side == ESide::Top) {
        GetCell(col, row).x0_30_gateBottom = true;
        GetCell(prevCol, prevRow).x0_28_gateTop = true;
      } else if (side == ESide::Right) {
        GetCell(col, row).x0_31_gateLeft = true;
        GetCell(prevCol, prevRow).x0_29_gateRight = true;
      } else if (side == ESide::Left) {
        GetCell(col, row).x0_29_gateRight = true;
        GetCell(prevCol, prevRow).x0_31_gateLeft = true;
      }
    }

    s32 nextCol = col;
    s32 nextRow = -1;
    if (row < 1 || side == ESide::Bottom || !GetCell(col, row).x0_24_openTop || !GetCell(col, row - 1).x1_25_onPath) {
      if (row < skMazeRows - 1 && side != ESide::Top && GetCell(col, row).x0_26_openBottom &&
          GetCell(col, row + 1).x1_25_onPath) {
        side = ESide::Bottom;
        nextRow = row + 1;
      } else {
        nextRow = row;
        if (col < 1 || side == ESide::Right || !GetCell(col, row).x0_27_openLeft ||
            !GetCell(col - 1, row).x1_25_onPath) {
          if (col > skMazeRows || side == ESide::Left || !GetCell(col, row).x0_25_openRight ||
              !GetCell(col + 1, row).x1_25_onPath) {
            return;
          }
          side = ESide::Right;
          nextCol = col + 1;
        } else {
          side = ESide::Left;
          nextCol = col - 1;
        }
      }
    } else {
      side = ESide::Top;
      nextRow = row - 1;
    }

    if (idx == puddle1Idx || idx == puddle2Idx) {
      if (col == 0 || row == 0 || col == skMazeCols - 1 || row == skMazeRows - 1) {
        if (idx == puddle1Idx) {
          puddle1Idx++;
        } else {
          puddle2Idx++;
        }
      } else {
        auto &cell = GetCell(col, row);
        cell.x1_24_puddle = true;
        if (side == ESide::Bottom) {
          GetCell(nextCol, nextRow).x0_24_openTop = false;
          cell.x0_26_openBottom = false;
        } else if (side == ESide::Top) {
          GetCell(nextCol, nextRow).x0_26_openBottom = false;
          cell.x0_24_openTop = false;
        } else if (side == ESide::Right) {
          GetCell(nextCol, nextRow).x0_27_openLeft = false;
          cell.x0_25_openRight = false;
        } else if (side == ESide::Left) {
          GetCell(nextCol, nextRow).x0_25_openRight = false;
          cell.x0_27_openLeft = false;
        }
      }
    }

    idx++;
    prevCol = col;
    prevRow = row;
    col = nextCol;
    row = nextRow;
  };
}

std::string CMazeState::to_str() {
  std::string res;


  for (int row = 0; row < skMazeRows; row++) {
    // first row
    for (int col = 0; col < skMazeCols; col++) {
      auto cell = this->x4_cells[col + row * skMazeCols];
      res += "╋";
      if (cell.x0_28_gateTop) {
        res += "┅";
      } else if (cell.x0_24_openTop) {
        res += " ";
      } else {
        res += "━";
      }
    }
    res += "╋\n";
    // center row
    for (int col = 0; col < skMazeCols; col++) {
      auto cell = this->x4_cells[col + row * skMazeCols];
      if (cell.x0_31_gateLeft) {
        res += "┇";
      } else if (cell.x0_27_openLeft) {
        res += " ";
      } else {
        res += "┃";
      }
      if (row == x88_enterRow && col == x84_enterCol) {
        res += "E";
      } else if (row == x90_targetRow && col == x8c_targetCol) {
        res += "X";
      } else if (cell.x1_24_puddle) {
        res += "O";
      } else {
        res += " ";
      }
    }
    {
      // last one
      auto cell = this->x4_cells[(skMazeCols - 1) + row * skMazeCols];
      if (cell.x0_29_gateRight) {
        res += "┇";
      } else if (cell.x0_25_openRight) {
        res += " ";
      } else {
        res += "┃";
      }
    }
    res += "\n";
  }

  // last row
  for (int col = 0; col < skMazeCols; col++) {
    auto cell = this->x4_cells[col + (skMazeRows - 1) * skMazeCols];
    res += "╋";
    if (cell.x0_30_gateBottom) {
      res += "┅";
    } else if (cell.x0_26_openBottom) {
      res += " ";
    } else {
      res += "━";
    }
  }
  res += "╋";

  return res;
}

//void CMazeState::DebugRender() {
//  m_renderer.Reset();
//  m_renderer.AddVertex(sDebugCellPos[skEnterCol + skEnterRow * skMazeCols], zeus::skBlue, 2.f);
//  for (s32 i = m_path.size() - 1; i >= 0; --i) {
//    s32 idx = m_path[i];
//    zeus::CVector3f pos;
//    if (idx == skMazeCols - 1) {
//      // 8,0 has no node, infer from 8,1
//      pos = sDebugCellPos[idx + skMazeCols] + zeus::CVector3f{4.f, 0.f, 0.f};
//    } else {
//      pos = sDebugCellPos[idx];
//    }
//    m_renderer.AddVertex(pos, zeus::skBlue, 2.f);
//  }
//  m_renderer.Render();
//}
