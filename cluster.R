#Cambiar el directorio para que ande

setwd("/home/marcos/Desktop/work") 
train <- read.csv("train.csv")
train <- data.frame(train)
train_f <- train[train$Y < 38,]
train_f <- train_f[train_f$X < -121,]
columnas <- names(train_f) %in% c("Category", "DayOfWeek", "Dates", "PdDistrict", "Resolution", "Address", "Descript") 
train_f <- train_f[!columnas]
summary(train_f)

# Determine number of clusters
wss <- (nrow(train_f)-1)*sum(apply(train_f,2,var))
for (i in 2:15) wss[i] <- sum(kmeans(train_f, centers=i)$withinss)
plot(1:15, wss, type="b", xlab="Number of Clusters", ylab="Within groups sum of squares")

# K-Means Cluster Analysis
fit <- kmeans(train_f, 3) # 3 cluster solution
# get cluster means
aggregate(train_f,by=list(fit$cluster),FUN=mean)
# append cluster assignment
train_3c <- data.frame(train_f, fit$cluster) 

# K-Means Cluster Analysis
fit <- kmeans(train_f, 4) # 4 cluster solution
# get cluster means
aggregate(train_f,by=list(fit$cluster),FUN=mean)
# append cluster assignment
train_4c <- data.frame(train_f, fit$cluster) 


# vary parameters for most readable graph
library(cluster)
clusplot(train_4c, fit$cluster, color=TRUE, shade=TRUE, labels=2, lines=0)

# Centroid Plot against 1st 2 discriminant functions
library(fpc)
plotcluster(train_4c, fit$cluster) 