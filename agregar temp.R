install.packages('weatherData')
library(weatherData)
# ejemplo temps = getWeatherForDate("SFO", "2014-05-05")

train <- read.csv("train.csv")

current_date = ""
current_temp = 0
z = ""

#descarga la info si no habia sido descargada ya
train$Weather <- sapply(train$Dates, FUN=function(h) {
  h <- substr(h, 1, 10)
  if (h != current_date) {
    assign("current_date", h, envir = .GlobalEnv)
    z <- getWeatherForDate("SFO", current_date)[1,3]
    assign("current_temp", z, envir = .GlobalEnv)
    return (z)
  }
  return (current_temp)
})

#saca las temps de su lista
train$Weather <- sapply(train$Weather, FUN=function(h) {as.numeric(unlist(h))})

#transforma los errores en "NA" OJO los errores son prqeu no se pudo conectar, no porque el dato no estuviera!

train$Weather <- sapply(train$Weather, FUN=function(h) {
  if (length(h)<=0) {
    return ("NA")
  }
  return (h)
})

write.csv(train, file = "trainweather.csv", quote =c(3,6), row.names=FALSE)
#quote es para que le ponga comillas solo a las cols 3 y 6 son desc y resultdo

