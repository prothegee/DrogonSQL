# Drogon framework SQL example

__Note:__
- Check your config test/deploy on `/configs/config.json` address and port listeners
- By default it will run at `http://127.0.0.1:8000`

<br>

---

__Prerequisite:__
- `drogonframework >= 1.7.4`
- `C++20 compiler support`
- `db_clients` might need to re-adjust

<br>

---

__Cmake build args help:__
- GNU\Linux
```bash
# inside /build directory
cmake -DCMAKE_CXX_FLAGS="-std=c++20 -fcoroutines" .. && cmake --build .
```
- Windows
```powershell
# inside /build directory
cmake -DCMAKE_CXX_FLAGS="/std:c++latest" .. && cmake --build .
```
`might require "/EHsc /EHa" exception handling syncrounus or asyncrounus for Windows build`

<br>

---

__Database tables:__
- Exported data tables are under `/models` directory, may be imported for example

<br>

---

###### end of readme
