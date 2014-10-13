-- recursively make deep copy of table
function deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else
        copy = orig
    end
    return copy
end

-- Counds how many surroundables surround point x, y
function surround (map, x, y, xdim, ydim, surroundable)
  local surrounding = 0
  
  if x-1 > 0 then
    if map[y][x-1] == surroundable then
      surrounding = surrounding + 1
    end
  end 

  if x+1 < xdim then
    if map[y][x+1] == surroundable then
      surrounding = surrounding + 1
    end
  end

  if y-1 > 0 then
    if map[y-1][x] == surroundable then
      surrounding = surrounding + 1
    end
  end

  if y+1 < ydim then
    if map[y+1][x] == surroundable then
      surrounding = surrounding + 1
    end
  end

  return surrounding;
end

-- insertValue is value to generate
-- checkValue is the value that will get turned into insertValue
function passMap(map, xdim, ydim, passes, insertValue, checkValue)
  local returnableMap = deepcopy(map)
  for i= 0,passes do
    local matrixcp = deepcopy(returnableMap)

      for y = 0, ydim-1 do
        for x = 0, xdim-1 do
          if returnableMap[y][x] == checkValue then
            n = surround(returnableMap, x, y, dim, dim, insertValue)
            pn = n / 4
            if math.random()*100 < pn * 100 then
              matrixcp[y][x] = insertValue
            end
          end
        end
      end
      
      returnableMap = deepcopy(matrixcp)
  end
  return returnableMap
end

matrix = {};

dim = 11

seedStart = 4
seedEnd  = 6

math.randomseed(os.time())


-- Create seed map
for y = 0,dim-1 do
 
  matrix[y] = {}
  for x = 0,dim-1 do
    if x >= seedStart and x <= seedEnd and y >= seedStart and y <= seedEnd then
      matrix[y][x] = 1;
    else
      matrix[y][x] = 0;
    end
  end
end

-- Generate Island
matrix = passMap(matrix, dim, dim, 6, 1, 0)

-- Place random mountain seeds
numMountains = 2;
mountainsPlaced = 0;
while mountainsPlaced < numMountains do
  local x = math.floor(math.random()*dim);
  local y = math.floor(math.random()*dim);
  if matrix[y][x] == 1 then
    matrix[y][x] = 2
    mountainsPlaced = mountainsPlaced + 1;
  end
end

-- Generate Island Mountains
matrix = passMap(matrix, dim, dim, 3, 2, 1)

-- Draw map to console
for y = 0,dim-1 do
  for x = 0,dim-1 do
    if matrix[y][x] < 100 then
      io.write(' ');
    end
    if matrix[y][x] == 1 then
      io.write('#');
    elseif matrix[y][x] == 2 then
      io.write('.');
    else
      io.write(' ');
    end
  end
  print()
end

-- C Hook
function retreiveMapValue(x, y)
  return matrix[y][x]
end
