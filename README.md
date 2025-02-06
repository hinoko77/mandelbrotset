# **Mandelbrot Fractal Viewer**

## **Overview**

This is an **OpenGL-based Mandelbrot Set Viewer** written in **C** using the **GLUT (OpenGL Utility Toolkit)**. The program allows users to visualize and interact with the Mandelbrot set, including zooming in and out. It features a guide page explaining controls before rendering the fractal.

---

## **Features**

- **Interactive Mandelbrot Set Visualization**
- **Zooming in and out** using the mouse
- **Smooth zoom animation**
- **Complex number display** (shows the current complex number being explored)
- **Guide Page** explaining controls before the fractal view

---

## **Installation and Dependencies**

### **Required Libraries**

Before compiling, ensure you have the following libraries installed:

- **OpenGL (********`libGL`********\*\*\*\* / ************`mesa-common-dev`************)**
- **GLUT (********`freeglut3-dev`********\*\*\*\*)**
- **GLU (********`libglu1-mesa-dev`********\*\*\*\*)**

### **Installation on Linux (Debian-based)**

```sh
sudo apt update
sudo apt install freeglut3-dev mesa-common-dev libglu1-mesa-dev
```

### **Installation on macOS**

Install **GLUT and OpenGL** using Homebrew:

```sh
brew install freeglut
```

### **Installation on Windows**

You need:

- **MinGW with OpenGL libraries**
- FreeGLUT (`freeglut.dll` should be in your project directory)

---

## **Compilation**

### **On Linux/macOS**

Use `gcc` to compile:

```sh
gcc -o mandelbrot mandel.c -lGL -lGLU -lglut -lm
```

### **On Windows (MinGW)**

```sh
gcc -o mandelbrot.exe mandel.c -lfreeglut -lopengl32 -lglu32 -lm
```

---

## **Running the Program**

After compiling, run the program:

```sh
./mandelbrot
```

For Windows:

```sh
mandelbrot.exe
```

---

## **Usage and Controls**

### **Guide Page**

When the program starts, it displays a **guide page** with basic instructions. Press **Enter** to start the Mandelbrot visualization.

### **Navigation Controls**

- **Left Click:** **Zoom In** (centers on the clicked point)
- **Right Click:** **Zoom Out**
- **Enter Key:** Switches from **Guide Page** to **Fractal Mode**

---

## **Code Breakdown**

### **1. Mandelbrot Calculation (********`fractal`********\*\*\*\* function)**

The Mandelbrot set is generated by iterating the equation:
\(Z_{n+1} = Z_n^2 + C\)
Where **Z** is a complex number and **C** represents the coordinates of each pixel.

Each pixel is checked for whether it **escapes** a threshold after a maximum number of iterations (`MAXCOUNT = 100`). The escape time determines the **color**.

### **2. Color Generation (********`HSLtoRGB`********\*\*\*\* function)**

- The color of each pixel is determined using **HSL (Hue-Saturation-Lightness) color mapping**.
- The hue is based on the **iteration count** and zoom level.
- The result is converted to **RGB values** to be rendered.

### **3. Zooming (********`handleMouse`********\*\*\*\* & ************`updateZoom`************ functions)**

- Clicking **zooms in** or **zooms out** by adjusting the **left, top, xside, and yside** values.
- The zoom transition is **animated** smoothly over `550ms`.

### **4. Guide Page (********`displayGuide`********\*\*\*\* function)**

- Displays **instructions** before the fractal view.
- Press **Enter** to proceed to Mandelbrot rendering.

---

## **Customization**

- Change **zoom speed**: Adjust `ZOOM_DURATION` in milliseconds.
- Modify **iteration count** (controls detail level): Change `MAXCOUNT`.
- Adjust **initial view**: Modify `left`, `top`, `xside`, and `yside`.

---

## **Screenshots**

🔹 
![](scrnshots/zoom1.png)

![](scrnshots/zoom2.png)

---

## **Potential Improvements**

✅ **Keyboard-based navigation** (arrow keys for panning)\
✅ **Save fractal images**\
✅ **Add Julia set rendering**

---

## **Author**

- Developed by hinokonoko77
- **License**: MIT License

---
