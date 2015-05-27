calculateDistancePoints = function(p1, p2)
    sqrt((p1[1] - p2[1])^2 + (p1[2] - p2[2])^2)

calculateLineLength = function(line)
{
    qtde_points = dim(line[1]@lines[[1]]@Lines[[1]]@coords)[1]
    coords = line[1]@lines[[1]]@Lines[[1]]@coords

    d = 0
    for(k in 1:(qtde_points - 1))
    {
        d = d + calculateDistancePoints(coords[k,], coords[k+1,])
    }
    return(d)
}

calculateLineLengthCoord = function(line)
{
    qtde_points = dim(line)[1]
    coords = line

    d = 0
    for(k in 1:(qtde_points - 1))
    {
        d = d + calculateDistancePoints(coords[k,], coords[k+1,])
    }
    return(d)
}

# distancia entre p0 e o segmento s1->s2
calculateDistancePointSegment = function(p, segA, segB)
{
    p2 = c(segB[1] - segA[1], segB[2] - segA[2]);
    something = p2[1]*p2[1] + p2[2]*p2[2];
    
    #Raian: Se something for 0, entao segA=segB. Retorna o x e o y do proprio ponto "segA"
    if(something == 0)
    {
    	#na verdade --> segA = segB
    	x = segA[1]
    	y = segA[2]
    }
    else
    {
		u = ((p[1] - segA[1]) * p2[1] + (p[2] - segA[2]) * p2[2]) / something;
	
		if (u > 1)
			u = 1
		else if (u < 0)
			u = 0;

		x = segA[1] + u * p2[ 1];
		y = segA[2] + u * p2[2];
	}

    dx = x - p[1]
    dy = y - p[2]

    dist = sqrt(dx*dx + dy*dy)

    return (dist)
}

calculatePointFromPointSegment = function(p, segA, segB, dist)
{
    dpb = calculateDistancePoints(p, segB)
    dpa = calculateDistancePoints(p, segA)

    if(dpb > dpa) { dpb = dpa; other = segB; segB=segA; segA = other }

    k = sqrt(dpb^2 - dist^2)
    	
    dab = calculateDistancePoints(segA, segB)
    
    if(dab != 0 && !is.na(k))
    {
		ly = (k * (segA[2] - segB[2]))/dab
		lx = (k * (segA[1] - segB[1]))/dab
	}
	else
	{
		ly = segA[2]
		lx = segA[1]
	}
		
    return(c(segB[1] + lx, segB[2] + ly))
}

firstPointOfLine = function(lines) 
    lines@lines[[1]]@Lines[[1]]@coords[1,]

lastPointOfLine = function(lines)
    lines@lines[[1]]@Lines[[1]]@coords[dim(lines@lines[[1]]@Lines[[1]]@coords)[1],]

addEntryPoints = function(spatiallines, points)
{
    lines_ids = getID(spatiallines)

    count_new_lines = 0

    for(i in 1:(dim(points)[1]))
    {
        point = points[i,]@coords

        dd = 1e+15
        pos = 0
        line = 0
        for (j in 1:length(lines_ids))
        {
            qtde_points = dim(spatiallines@lines[[j]]@Lines[[1]]@coords)[1]
            coords = spatiallines@lines[[j]]@Lines[[1]]@coords

            for(k in 1:(qtde_points - 1))
            {
                d = aRT:::calculateDistancePointSegment(point, coords[k,], coords[k+1,])
                if (d < dd)
                {
                    dd = d
                    kk = k
                    jj = j
                }
            }
        }
        
        coords = spatiallines@lines[[jj]]@Lines[[1]]@coords

        pto = aRT:::calculatePointFromPointSegment(point, coords[kk,], coords[kk+1,], dd)

        line1 = rbind(coords[1:kk,], pto)
        
        condition = (pto[1] == coords[kk,1] && pto[2] == coords[kk,2])
		if (!is.na(condition) && condition)
		{
			.catSilent("EQUAL A: PROBLEM!\n")
		}
        line2 = rbind(pto, coords[(kk+1):(dim(coords)[1]),])
        
        condition2 = pto[1] == coords[kk+1,1] && pto[2] == coords[kk+1,2]
		#if (pto[1] == coords[kk+1,1] && pto[2] == coords[kk+1,2])
		if(!is.na(condition2) && condition2)
		{
			line2 = coords[(kk+1):(dim(coords)[1]),]
		}

		if(length(line2) != 2)
		{
        	l = Line(line2)
        	l = Lines(l, paste(count_new_lines+10000))
        	count_new_lines = count_new_lines + 1
        	spatiallines@lines[[length(lines_ids)+count_new_lines]] = l
        	spatiallines@lines[[jj]]@Lines[[1]]@coords = line1
		}
    }
    return(spatiallines)
}

######################################################################################################
## calcula os pontos extremos de cada linha, colocando em uma lista com a contagem
## de repeticoes destes pontos

calculateNodes = function(spatiallines)
{
    point2char = function(point) paste(point[1],"..",point[2],sep="")

    # calcula estes tres elementos
    xx = c()
    yy = c()
    netpoints = vector()

    ids = getID(spatiallines)

    count = 1
    for (i in ids)
    {
        point = firstPointOfLine(spatiallines[i])
        name = point2char(point)
        if(is.na(netpoints[name]))
        {
            netpoints[name] = count
            xx[count] = point[1]
            yy[count] = point[2]
            count = count + 1
        }
        point = lastPointOfLine(spatiallines[i])
        name = point2char(point)
        if(is.na(netpoints[name]))
        {
            netpoints[name] = count
            xx[count] = point[1]
            yy[count] = point[2]
            count = count + 1
        }
    }

    pts = cbind(xx, yy)
    sp = SpatialPoints(pts)
    nodes = SpatialPointsDataFrame(sp, data=data.frame(ID=paste(1:length(xx))))
    return(list(nodes=nodes, netpoints=netpoints))
}

initNodesDistance = function(nodes, origins)
{
    nodes@data[,"distance"] = 1e+15
    nodes@data[,"target_id"] = ""

    for(i in 1:(dim(origins)[1]))
    {
        distance = 1e+15
        origin = origins[i,]@coords
        pos = 0

        for(j in 1:(dim(nodes)[1]))
        {
            point = nodes[j,]@coords
            d = calculateDistancePoints(origin, point)
            if (d < distance)
            {
                distance = d
                pos = j
            }
        }
        nodes@data[pos,"distance"] = distance
        nodes@data[pos,"target_id"] = as.character(origins@data[i,"ID"])
    }
    return(nodes)
}

createConnectivityMatrix = function(slines, netpoints, weightf)
{
    point2char = function(point) paste(point[1],"..",point[2],sep="")
    
    ids = getID(slines)

    size = length(netpoints)
    m = matrix(1e+15, nrow = size, ncol = size)
   
    for (i in 1:length(ids))
    {
        point1 = firstPointOfLine(slines[i])
        name1 = point2char(point1)

        dist = weightf(calculateLineLength(slines[i]), ids[i])
        point2 = lastPointOfLine(slines[i])
        name2 = point2char(point2)

        # matriz simetrica
        m[netpoints[name1][[1]], netpoints[name2][[1]]] = dist
        m[netpoints[name2][[1]], netpoints[name1][[1]]] = dist
    }
    return(m)
}

updateDistances = function(nodes, connect_matrix)
{
	reduce = TRUE
	while(reduce)
	{
		reduce = FALSE
		for(i in 1:(dim(nodes)[1]))
		{
			dist = nodes@data[i,"distance"]
			newdists = connect_matrix[i,] + nodes@data[,"distance"]
			j = which.min(newdists)
			newdist = newdists[j]

			if(dist > newdist)
			{
				nodes@data[i,"distance"] = newdist
				nodes@data[i,"target_id"] = nodes@data[j,"target_id"]
				reduce = TRUE
			}
		}
	}
	return(nodes)
}

getNeighborsOpenNetworkFunction = function(points, network)
{
	spatiallines = network$network
	centroide_lotes = points
	mynodes = network$nodes
	netpoints = network$references
	origin_ids  = getID(centroide_lotes)
	lines_ids = getID(spatiallines)

	return (function(id)
	{
		idpos = which(origin_ids == id)
	    point = centroide_lotes[idpos,]@coords
	    dd = 1e+15
	    pos = 0
	    line = 0
	    for (j in 1:length(lines_ids))
	    {
	        qtde_points = dim(spatiallines@lines[[j]]@Lines[[1]]@coords)[1]
	        coords = spatiallines@lines[[j]]@Lines[[1]]@coords
	        for(k in 1:(qtde_points - 1))
	        {
	            d = calculateDistancePointSegment(point, coords[k,], coords[k+1,])
	            if (d < dd)	
	            {
	                dd = d
					kk = k
	   	            jj = j
	            }
	        }
	    }

	    coords = spatiallines@lines[[jj]]@Lines[[1]]@coords
	
	    pto = calculatePointFromPointSegment(point, coords[kk,], coords[kk+1,], dd)
	
    	line1 = rbind(coords[1:kk,], pto)
	    line2 = rbind(pto, coords[(kk+1):(dim(coords)[1]),])

	    len1 = calculateLineLengthCoord(line1)
	    len2 = calculateLineLengthCoord(line2)

	    point2char = function(point) paste(point[1],"..",point[2],sep="")

	    ch1 = point2char(coords[1,])
	    dist1 = mynodes@data[netpoints[ch1],"distance"] + len1

	    ch2 = point2char(coords[dim(coords)[1],])
	    dist2 = mynodes@data[netpoints[ch2],"distance"] + len2

	    if (dist1 < dist2)
	    {
		    com1 = mynodes@data[netpoints[ch1],"target_id"]
			return(list(ids=com1, distance=dist1))
	    }    else    {
		    com2 = mynodes@data[netpoints[ch2],"target_id"]
			return(list(ids=com2, distance=dist2))
	    }
	})
}

getNeighborsEuclideanDistanceFunction = function(origin_points, destiny_points)
{
	origin_ids  = getID(origin_points)
	destiny_ids = getID(destiny_points)

	return (function(id)
	{
		pos = which(origin_ids == id)
	    point1 = origin_points[pos,]@coords
	    dd = 1e+15
	    pos = 0
	    for (j in 1:length(destiny_ids))
	    {
		    point2 = destiny_points[j,]@coords
	
			d = calculateDistancePoints(point1, point2)
	
            if (d < dd)	
            {
                dd = d
   	            pos = j
	        }
	    }

		neighbor_id = as.character(destiny_points@data[pos,"ID"])
		return (list(ids = neighbor_id, distance = dd))
	})
}


getNeighborsEuclideanMaxDistanceFunction = function(points, max_dist)
{
	ids  = getID(points)
	
	return (function(id)
	{
		pos = which(ids == id)
	    point1 = points[pos,]@coords
		qtde = 0
		result_ids = vector()
		result_distances = vector()

	    for (j in 1:length(ids))
	    {
			if(j != pos)
			{
			    point2 = points[j,]@coords

				d = calculateDistancePoints(point1, point2)
	
	            if (d < max_dist)	
	            {
					qtde = qtde + 1
					result_ids = c(result_ids, as.character(points@data[j,"ID"])) ##       ids[j])
					result_distances = c(result_distances, d)
		        }
		    }
		}

		return (list(ids = result_ids, distance = result_distances))
	})
}

## TODO: existe um erro aqui quando um dos pontos de destino se conectam a um dos dois
## pontos extremos de uma linha. provavelmente esta acontecendo uma divisao por zero.
createOpenNetwork = function(destiny_points, network, weight_function)
{
	.catSilent("Step 1/5 (adding points) ... ")
	full_network = addEntryPoints(network, destiny_points)
	
	.catSilent("Yes\nStep 2/5 (generating network) ... ")
	x = calculateNodes(full_network)
	network_nodes = x$nodes
	network_references = x$netpoints

	.catSilent("Yes\nStep 3/5 (starting distances) ... ")
	full_mynodes = initNodesDistance(network_nodes, destiny_points)

	.catSilent("Yes\nStep 4/5 (creating connectivity matrix) ... ")
	connectivity_matrix = createConnectivityMatrix(full_network, network_references, weight_function)

	.catSilent("Yes\nStep 5/5 (updating distances) ... ")
	mynodes_initial = updateDistances(full_mynodes, connectivity_matrix)
	.catSilent("Yes\n")

	return(list(network=full_network, nodes = mynodes_initial, references = network_references))
}

connectToBiggerIntersectionArea = function(spcells, lpolygons)
{
	artcells = as.aRTgeometry(spcells)
	id_cells = getID(spcells)
	table_cells = data.frame(id=id_cells, area=0, father="", stringsAsFactors=FALSE)
	rownames(table_cells) = id_cells

	q = openQuerier(lpolygons)
	for(i in 1:(summary(q)$elements))
	{
		d = getData(q, 1, as.sp=FALSE)
		father = getID(getGeometry(d))
		clip = getClip(artcells, d)
		if(!is.null(clip))
		{
			ids = getID(clip)
			for(j in 1:length(ids))
			{
				idj = ids[j]
				area = clip@polygons[[j]]@area
				if(table_cells[idj,"area"] < area)
				{
					table_cells[idj, "area"] = area
					table_cells[idj, "father"] = father
				}
			}
		}
	}
	return(table_cells)
}

setMethod("createGPM", "aRTlayer", function(object, get_neighbors)
{
	ids = getID(object)

	result = list()

	cont = 1
	for(i in ids)
	{
		.catSilent(i, "\n")
		rel = get_neighbors(i)
		result[[i]] = rel
		cont = cont + 1
	}
	return (result)
})

## Raian: Connect a cell intersection lines
getNeighborsIntersectionLines = function(lcells, lalines)
{
 	return(function(id)
 	{
		  parents = getRelation(lalines, c("within", "crosses"), lcells, id)
		  return(list(ids=parents))
 	})

}

## Raian: Connect a cell with contained points
getNeighborsContainedPoints = function(lcells, lapoints)
{
 	return(function(id)
 	{
		parents = getRelation(lapoints, "contains", lcells, id)
		return(list(ids=parents))
 	})
}

## Raian: Connect a line with intersection polygons
connectLineToIntersectionPolygons = function(lalines, lpolygons)
{
 	return(function(id)
 	{
		parents = getRelation(lpolygons, c("within", "crosses"), lalines, id)
		return(list(ids=parents))
 	})
}

## Novo save para a GPM - Raian
## file extensions supported: .gpm, .gal and .gwt
saveGPM = function(gpm, filename, layer1, layer2=NULL, key="object_id_", attrib=NULL)
{
	tkns = unlist(strsplit(filename, "\\."))
	lenTkns = length(tkns)
	extension = tkns[lenTkns]

	.catSilent("Saving the GPM in a \".", extension, "\" file...", "\n", sep="")

	if(identical(extension, "gpm"))
	{
	    ids = names(gpm)
	    qtde = length(ids)

	    zz <- file(filename, "w")
	    
	    if(is.null(layer2))
		    layer2 = layer1;

	    attribs = names(gpm[[ids[1]]])
	    countAttribs = 0
	    numAttribs = length(attribs) - 1

	    cat(c(numAttribs, "\t", layer1, "\t", layer2), "\t", sep="", file=zz)
	    
	    for(i in attribs)
	    {
		    if(identical(i, "ids") == FALSE)
			 cat(i, "\t", sep="", file=zz)
	    }
	    cat("\n", file=zz);

	    for (i in ids)
	    {
		    cat(i, "\t", sep="", file=zz)                   		 # ID
		    
		    if(length(gpm[[i]]$ids[1]) != 0)
		    {
			  if(gpm[[i]]$ids[1] != "") 		# if has no neighbors, print 0
			  {
				  cat(length(gpm[[i]]$ids), "\n", sep="", file=zz) # size
				  
				  idNeighs = gpm[[i]]$ids;
				  count = 1;
				  
				  for(j in idNeighs)
				  {
					  cat(j, "\t", sep="", file=zz)   # neighbors
					  if(length(attribs) > 1)
					  {
						  for(k in attribs)
						  {
						      if(identical(k, "ids") == FALSE)
							  cat(gpm[[i]][[k]][count], "\t", sep="", file=zz);	#weights
						  }
					  }
					  count = count + 1;
				  }
				  cat("\n", file=zz);
			  }
			  else
				  cat(0, "\n", sep="", file=zz)
		    }
		    else
			  cat(0, "\n", sep="", file=zz)
	    }
	    close(zz)
	}
	else if(identical(extension, "gal") || identical(extension, "GAL"))
	{
	    if(is.null(layer2))
	    {
		    ids = names(gpm)
		    qtde = length(ids)

		    zz <- file(filename, "w")

		    cat(c(0, qtde, layer1, key), "\n", sep="\t", file=zz) # Size, Layer and key variable

		    for (i in ids)
		    {
			    cat(i, "\t", file=zz)                   		 # ID
			    if(gpm[[i]]$ids[1] != "")
			    {
				    cat(length(gpm[[i]]$ids), sep="\n", file=zz) # size
				    cat(gpm[[i]]$ids, "\n", sep="\t", file=zz)   # neighbors
			    }
			    else
				    cat(0, sep="\n", file=zz)
		    }
		    close(zz)
	     }
	     else
		    stop("GAL format file does not support neighborhoods between two layers. Use the GPM format.")
	}
	else if(identical(extension, "gwt") || identical(extension, "GWT"))
	{
	    if(is.null(layer2))
	    {
		    ids = names(gpm)
		    qtde = length(ids)

		    zz <- file(filename, "w")

		    cat(c(0, qtde, layer1, key), "\n", sep="\t", file=zz)              # size

		    for (i in ids)
		    {
			    idNeighs = gpm[[i]]$ids
			    count = 1
			    for(j in idNeighs)
			    {
				    cat(i, "\t", sep="", file=zz)                    # ID cell
				    cat(j, "\t", sep="", file=zz) 					 # ID neighbor
				    if(!is.null(attrib))
					    cat(gpm[[i]][[attrib]][count], file=zz) # distance 
												  # 
				    count = count + 1
				    cat("\n", file=zz)
			    }
		    }
		    close(zz)
	    }
	    else
		    stop("GWT format file does not support neighborhoods between two layers. Use the GPM format.")
	}
	else
	{
	    stop("Extension not supported!!! The extensions supported are: \".gpm\", \".gal\"/\".GAL\" and \".gwt\"/\".GWT\"");
	}

	.catSilent("GPM successfully saved!", "\n\n", sep="")
}


