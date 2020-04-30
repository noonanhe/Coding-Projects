#
# This is a Shiny web application. You can run the application by clicking
# the 'Run App' button above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/
#

library(shiny)
library(tidyverse)
library(DT)
library(plotly)
library(maps)
# A read-only data set that will load once, when Shiny starts, and will be
# available to each user session
jobs_data <- read.csv2("jobs_data.csv", stringsAsFactors = FALSE)
jobs_data$Posted <- factor(jobs_data$Posted, levels = c("Just posted", "Today", "1 day ago",
                                                        paste(2:29, "days ago"), "30+ days ago"))

# A non-reactive function that will be available to each user session
long.limits <- function(map) {
  2.496881/(diff(range(map$long))/diff(range(map$lat)))*
    (range(map$long) - sum(range(map$long))/2) + sum(range(map$long))/2
}

# Define UI for application that draws a histogram
ui <- fluidPage(

  titlePanel("Data Science Jobs"),
  
  sidebarLayout(
    
    sidebarPanel(
      selectInput("state","Location",choices = c(jobs_data$state.name, ""),selected ="" ),
      selectInput("post", "Posted", choices = c(levels(jobs_data$Posted), ""), selected =""),
      checkboxInput("intern", "Internship", value =FALSE)
    ),
    
    mainPanel(
      tabsetPanel(
        tabPanel("Map", plotlyOutput("map", width = 600, height = 350), DTOutput("selected")),
        
        tabPanel("Postings", DTOutput("postings_table"))
      )
      
    )
  )
  
)

# Define server logic required to draw a histogram
server <- function(input, output) {
  subset_data <- reactive({

    jobs_subset <- jobs_data
    if (input$intern) {
      jobs_subset <- jobs_subset %>%
        filter(grepl("intern", tolower(Title)))
    } 
    if (input$post %in% unique(jobs_data$Posted)) {
      jobs_subset <- jobs_subset %>%
        filter(Posted == input$post)
    }
    if (input$state %in% unique(jobs_data$state.name)) {
      jobs_subset <- jobs_subset %>%
        filter(state.name == input$state)
    }
    
    jobs_subset
  
  })
  
  
  output$map <- renderPlotly({
    jobs_by_location <- subset_data() %>%
      group_by(lat, lon, Location) %>%
      summarise(Freq = n())
    
    map_info <- map_data('state', region = input$state)
    
    
    map_plot <- map_info %>%
      
      ggplot(aes(x=long, y=lat, group=group)) +
      geom_polygon(fill="lightgray", colour="black", alpha = 0.8) +
      geom_point(data=jobs_by_location, inherit.aes=F,
                 aes(x=lon, y=lat, size=Freq), colour="red", alpha=.8) +
      scale_radius(range = (range(jobs_by_location$Freq)-1)*5/846 + 1) +
      scale_x_continuous(limits = long.limits(map_info)) +
      theme(panel.background = element_blank(),
            axis.title = element_blank(),
            axis.ticks = element_blank(),
            axis.line = element_blank(),
            axis.text = element_blank())
  
  
    ggply <- ggplotly(map_plot, tooltip = "text")
    ggply$x$data[[1]]$hoverinfo <- "none"
    ggply$x$data[[2]]$hoverinfo <- "text"
    ggply$x$data[[2]]$text <- paste(jobs_by_location$Freq, 
                                      ifelse(jobs_by_location$Freq > 1, " positions", " position"),
                                      "\n", 
                                      jobs_by_location$Location, sep = "")
      
    ggply
  })
  
  output$postings_table <- renderDT({
    subset_data()%>%select(c(1:5))
  })
  
  output$selected <- renderDT({ 
    click <- event_data("plotly_click")
    req(click)
    if (click$curveNumber== 1){
      
       subset_data()%>%filter(lon == round(click$x,digits =5), lat == round(click$y,digits=5))%>%select(c("Title","Company","Location"))
      
    }
    })
}

# Run the application 
shinyApp(ui = ui, server = server)
