resp <- q27
n <- q27n
# prec <- c()
# rec <- c()
dcg <- c()
# rel <- 0

# ci <- 1
for (i in 1:length(resp)){
  # if (resp[i] >= 3){
  if (i == 1){
    dcg[i] <- resp[i]
  }
  else{
    dcg[i] <- dcg[i-1] + (resp[i]/log2(i))
  }
    # rel <- rel+1
    # prec[ci] <- rel/i
    # rec[ci] <- rel/n
    # ci <- ci+1
  # }
  # else{
  #   prec[i] <- prec[]
  #   rec[i] <- rel/n
  # }
}

plot(dcg,type='b',ylab="DCG",xlab="Índice")
