import matplotlib.pyplot as plt

x_ticks = [10,50,100,150,200,250,300,400,500]

r_star_40_tree_insertions = [0.16, 0.95, 2.27, 3.19, 4.11, 5, 3.44, 7.65, 9.6]
r_star_half_tree_insertions = [0.17, 0.97, 2.29, 3.21, 2.53, 5.08, 3.53, 7.85, 9.22]
r_linear_2_tree_insertions = [0.072, 0.14, 0.2, 0.24, 0.3, 0.34, 0.4, 0.51, 0.6]
r_linear_half_tree_insertions = [0.072, 0.14, 0.19, 0.23, 0.31, 0.34, 0.4, 0.51, 0.59]
r_quad_2_tree_insertions = [0.1, 0.25, 0.41, 0.53, 0.69, 0.83, 0.97, 1.27, 1.53]
r_quad_half_tree_insertions = [0.1, 0.25, 0.40, 0.51, 0.65, 0.80, 0.93, 1.19, 1.49]

r_star_40_tree_pages = [83.163, 14.972, 7.392, 4.863, 3.638, 2.918, 2.395, 1.819, 1.496]
r_star_half_tree_pages = [82.120, 14.882, 7.356, 4.857, 3.631, 2.915, 2.444, 1.840, 1.472]
r_linear_2_tree_pages = [83.496, 15.765, 8.068, 5.460, 4.149, 3.333, 2.780, 2.061, 1.629]
r_linear_half_tree_pages = [83.496, 15.765, 8.068, 5.460, 4.149, 3.333, 2.780, 2.061, 1.629]
r_quad_2_tree_pages = [88.311, 15.987, 7.764, 5.061, 3.808, 2.997, 2.494, 1.854, 1.476]
r_quad_half_tree_pages = [81.708, 14.825, 7.323, 4.879, 3.627, 2.969, 2.403, 1.787, 1.489]

r_star_40_tree_avg_rquery_ms = [0.72, 0.93, 0.81, 0.79, 0.77, 0.71, 0.9, 0.65, 0.6]
r_star_half_tree_avg_rquery_ms = [1.01, 0.99, 0.84, 0.85, 1.09, 0.73, 0.9, 0.71, 0.62]
r_linear_2_tree_avg_rquery_ms = [1.21, 2.38, 3.7, 3.47, 6.52, 5.2, 5.11, 7.22, 8.25]
r_linear_half_tree_avg_rquery_ms = [1.12, 2.48, 2.93, 3.29, 6.05, 4.95, 5.02, 7.37, 8.28]
r_quad_2_tree_avg_rquery_ms = [0.22, 0.15, 0.11, 0.13, 0.13, 0.12, 0.13, 0.13, 0.13]
r_quad_half_tree_avg_rquery_ms = [0.27, 0.16, 0.14, 0.13, 0.14, 0.12, 0.14, 0.16, 0.17]

r_star_40_tree_avg_rquery_touch = [633, 290, 191, 138, 103, 82, 67, 50, 38]
r_star_half_tree_avg_rquery_touch = [791, 310, 205, 144, 110, 86, 72, 50, 38]
r_linear_2_tree_avg_rquery_touch = [952, 876, 780, 605, 734, 618, 530, 572, 498]
r_linear_half_tree_avg_rquery_touch = [952, 876, 780, 605, 734, 618, 530, 572, 498]
r_quad_2_tree_avg_rquery_touch = [105, 29, 15, 12, 10, 9, 8, 7, 6]
r_quad_half_tree_avg_rquery_touch = [139, 30, 17, 14, 13, 10, 10, 10, 9]

plt.plot(x_ticks, r_star_40_tree_insertions, label="R* m=0.4M")
plt.plot(x_ticks, r_star_half_tree_insertions, label="R* m=M/2")
plt.plot(x_ticks, r_linear_2_tree_insertions, label="R-Lin m=2")
plt.plot(x_ticks, r_linear_half_tree_insertions, label="R-Lin m=M/2")
plt.plot(x_ticks, r_quad_2_tree_insertions, label="R-Quad m=2")
plt.plot(x_ticks, r_quad_half_tree_insertions, label="R-Quad m=M/2")

plt.ylabel("Minutes")
plt.xlabel("M Value")
plt.title("Minutes to insert")
plt.legend()
plt.xticks(x_ticks)
plt.show()

plt.plot(x_ticks, r_star_40_tree_pages, label="R* m=0.4M")
plt.plot(x_ticks, r_star_half_tree_pages, label="R* m=M/2")
plt.plot(x_ticks, r_linear_2_tree_pages, label="R-Lin m=2")
plt.plot(x_ticks, r_linear_half_tree_pages, label="R-Lin m=M/2")
plt.plot(x_ticks, r_quad_2_tree_pages, label="R-Quad m=2")
plt.plot(x_ticks, r_quad_half_tree_pages, label="R-Quad m=M/2")

plt.ylabel("Tree Pages(*1000)")
plt.xlabel("M value")
plt.title("Total Pages(*1000)")
plt.legend()
plt.xticks(x_ticks)
plt.show()

plt.plot(x_ticks, r_star_40_tree_avg_rquery_ms, label="R* m=0.4M")
plt.plot(x_ticks, r_star_half_tree_avg_rquery_ms, label="R* m=M/2")
plt.plot(x_ticks, r_linear_2_tree_avg_rquery_ms, label="R-Lin m=2")
plt.plot(x_ticks, r_linear_half_tree_avg_rquery_ms, label="R-Lin m=M/2")
plt.plot(x_ticks, r_quad_2_tree_avg_rquery_ms, label="R-Quad m=2")
plt.plot(x_ticks, r_quad_half_tree_avg_rquery_ms, label="R-Quad m=M/2")
plt.plot(x_ticks, [(143/10000) * 1000] * 9, label = 'Naive Algorithm')

plt.ylabel("ms")
plt.xlabel("M Value")
plt.title("Average Range Query time(in ms)")
plt.legend()
plt.xticks(x_ticks)
plt.show()

plt.plot(x_ticks, r_star_40_tree_avg_rquery_touch, label="R* m=0.4M")
plt.plot(x_ticks, r_star_half_tree_avg_rquery_touch, label="R* m=M/2")
plt.plot(x_ticks, r_linear_2_tree_avg_rquery_touch, label="R-Lin m=2")
plt.plot(x_ticks, r_linear_half_tree_avg_rquery_touch, label="R-Lin m=M/2")
plt.plot(x_ticks, r_quad_2_tree_avg_rquery_touch, label="R-Quad m=2")
plt.plot(x_ticks, r_quad_half_tree_avg_rquery_touch, label="R-Quad m=M/2")

plt.ylabel("Pages")
plt.xlabel("M Value")
plt.title("Average Range Query Page Touches")
plt.legend()
plt.xticks(x_ticks)
plt.show()
