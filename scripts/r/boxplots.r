# library
library(ggplot2)
library(plyr)
library(reshape2)
library(gridExtra)
library(ggpubr) 

# Parameters to change --------------------------------------------------------------------------------------

read_file  <- "***.csv"   # name of CSV to read
write_file <- "***"       # name of output file
location   <- "***"       # location of input and output file

d_width  <- 8.35
d_height <- 4


# -----------------------------------------------------------------------------------------------------------

file_location <- paste(location, name, sep="")
file_read <- paste(file_location, write_file, sep="")
file_write <- paste(file_location, write_file, sep="")

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

scale_grey_Publication <- function(...){
  library(scales)
  discrete_scale("colour","Publication",manual_pal(values = c("#ffffff","#ffffff","#ffffff","#ffffff","#ffffff","#ffffff","#ffffff","#ffffff","#ffff33", "#ffffff","#ffffff","#ffffff","#ffffff","#ffffff")), ...)
  
}

# Load Data -------------------------------------------------------------------------------------------------


lbox = read.csv(file_read,
                sep = ",",
                check.names=FALSE,
                header = T  # first line contains column names, so we can immediately call `lpoints$age`        
)

x <- lbox

quantile(x$Completion)

lbox$Sets <- reorder(lbox$Sets, -lbox$Completion)

# -----------------------------------------------------------------------------------------------------------

# Set a different color for each group
p <- ggplot(lbox, aes(x=lbox$Network, y=lbox$Completion,  fill=lbox$Network)) + 
  geom_boxplot(alpha=0.3) +
  xlab("") + ylab("Completion (%)") + 
  geom_boxplot() + 
  theme_Publication() +
  theme(
          legend.title=element_blank(), 
          legend.text = element_text(size = 16, face = "bold"),
          legend.spacing =  unit(10, "cm"),
          legend.key = element_rect(size = 5),
          legend.key.size = unit(1.5, 'lines'),
          legend.position = c(0.5, -0.1),
          legend.spacing.x = unit(0.5, 'cm'),
          axis.text.x=element_blank(),
          axis.ticks = element_blank()
        ) +
  stat_compare_means(label.y = 1, method = "t.test") + 
  scale_y_continuous(limits=c(0,100))

p <- p + facet_wrap( lbox$Sets ~ ., ncol=4)

p

p + scale_fill_manual(values=c("#a0a0a0", "#ffffff", "#56B4E9"))

# Save PNG
ggsave(file_write, width = d_width, height = d_height, units = "in")

# -----------------------------------------------------------------------------------------------------------


