# Contributing Guidelines

Hướng dẫn đóng góp code cho project.

## Quy trình làm việc

### Branch Strategy

```
main (protected)
  ├── dev (development)
  │   ├── feature/memory-management
  │   ├── feature/scheduler
  │   ├── feature/paging
  │   └── bugfix/page-fault
```

**Quy tắc đặt tên branch**:
- Feature: `feature/<tên-feature>`
- Bugfix: `bugfix/<tên-bug>`
- Hotfix: `hotfix/<tên-issue>`

### Workflow

```bash
git checkout -b feature/your-name
git add .
git commit -m "feat: description"
git push origin feature/your-name
```

## Commit Convention

**Types**: `feat`, `fix`, `docs`, `refactor`, `test`

**Example**: `feat(memory): implement alloc_pages_range`

## Code Style

**Indentation**: 4 spaces

**Naming**:
- Functions: `snake_case`
- Variables: `snake_case`
- Constants: `UPPER_CASE`
- Structs: `snake_case` with `_t` suffix

**Line length**: Max 100 characters
