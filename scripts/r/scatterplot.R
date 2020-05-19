library(devtools)
library(ggplot2)
library(gridExtra)

# Parameters to change --------------------------------------------------------------------------------------

read_file  <- "***.csv"   # name of CSV to read
write_file <- "***"       # name of output file
location   <- "***"       # location of input and output file

d_width  <- 8.35
d_height <- 4

# Loading Theme Data ----------------------------------------------------------------------------------------

theme_Publication <- function(base_size=14, base_family = "Helvetica") {
  library(grid)
  library(ggthemes)
  (theme_foundation(base_size=base_size, base_family=base_family)
    + theme(plot.title = element_text(face = "bold",
                                      size = rel(1.2), hjust = 0.5),
            text = element_text(),
            panel.background = element_rect(colour = NA),
            plot.background = element_rect(colour = NA),
            panel.border = element_rect(colour = NA),
            axis.title = element_text(face = "bold",size = rel(1)),
            axis.title.y = element_text(angle=90,vjust =2),
            axis.title.x = element_text(vjust = -0.2),
            axis.text = element_text(), 
            axis.line = element_line(colour="black"),
            axis.ticks = element_line(),
            panel.grid.major = element_line(colour="#f0f0f0"),
            panel.grid.minor = element_blank(),
            legend.key = element_rect(colour = NA),
            legend.position = "bottom",
            legend.direction = "horizontal",
            legend.key.size= unit(0.2, "cm"),
            legend.margin = unit(0, "cm"),
            legend.title = element_text(face="italic"),
            plot.margin=unit(c(10,5,5,5),"mm"),
            strip.background=element_rect(colour="#f0f0f0",fill="#f0f0f0"),
            strip.text = element_text(face="bold")
    ))
  
}

scale_fill_Publication <- function(...){
  library(scales)
  discrete_scale("fill","Publication",manual_pal(values = c("#386cb0","#fdb462","#7fc97f","#ef3b2c","#662506","#a6cee3","#fb9a99","#984ea3","#ffff33")), ...)
  
}

scale_colour_Publication <- function(...){
  library(scales)
  discrete_scale("colour","Publication",manual_pal(values = c("#386cb0","#fdb462","#7fc97f","#ef3b2c","#662506","#a6cee3","#fb9a99","#984ea3","#ffff33")), ...)
  
}

# -----------------------------------------------------------------------------------------------------------

file_location <- paste(location, read_file, sep="")

file_write_pdf <- paste(location, write_file, sep="")
file_write_png <- paste(location, write_file, sep="")

file_write_pdf <- paste(file_write_pdf, "-scatterPlot.pdf", sep="")
file_write_png <- paste(file_write_png, "-scatterPlot.png", sep="")

# Load Data --------------------------------------------------------------------------------------------------

# Load from CSV
scatter <- read.csv(file_location,
                sep = ",",
                check.names=FALSE,
                header = T  # first line contains column names, so we can immediately call `lpoints$age`        
)

# Aggregate data points which share the same X and Y
scatter <- aggregate(cbind(scatter$Depth) ~ scatter$Name + scatter$X + scatter$Y, 
                     data = scatter, sum, na.rm = TRUE)
# Rename Headers
names(scatter) <- c("Name","X", "Y", "Depth")

# Plot Graph -------------------------------------------------------------------------------------------------

# Loess method
p <- ggplot(scatter, aes(x=X, y=Y, color=Y, size=Depth)) + 
  geom_point(alpha=0.4) +
  scale_size_continuous( trans="exp", range=c(1, 10)) +
  scale_colour_continuous() +
  scale_color_distiller(palette = "Spectral") +
  geom_smooth(size=1, color="black", method = lm, formula = y ~ splines::bs(x, 3), se = FALSE) + 
  scale_fill_Publication() + 
  theme_Publication() + 
  xlab("Generation") + 
  ylab("Fitness") +
  scale_x_continuous(labels = function(x) format(x, scientific = TRUE)) +
  theme(
        legend.position="none", 
        legend.key = element_blank(),
        axis.text.x  = element_text(size=8),
        panel.margin.y = unit(1, "lines")
        )

p <- p + facet_wrap( ~ scatter$Name, ncol=1)

p

# Save PDF and PNG
ggsave(filename = file_write_pdf, width = d_width, height = d_height,            units = "in", device=cairo_pdf)
ggsave(filename = file_write_png, width = d_width, height = d_height, dpi = 300, units = "in", device='png')

# -----------------------------------------------------------------------------------------------------------

