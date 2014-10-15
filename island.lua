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

dim = 100

seedStart = 50
seedEnd  = 55

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
matrix = passMap(matrix, dim, dim, 55, 1, 0)

-- Place random mountain seeds
numMountains = 2;
mountainsPlaced = 0;
peaks = {}

while mountainsPlaced < numMountains do
  local x = math.floor(math.random()*dim);
  local y = math.floor(math.random()*dim);
  if matrix[y][x] == 1 then
    peaks[mountainsPlaced] = {['x']=x, ['y']=y};
    mountainsPlaced = mountainsPlaced + 1;
  end
end

-- Generate Island Mountains
matrix[peaks[0].y][peaks[0].x] = 4;
matrix[peaks[1].y][peaks[1].x] = 4;
matrix = passMap(matrix, dim, dim, 50, 4, 1)

-- Generate Island Mountains
matrix[peaks[0].y][peaks[0].x] = 2;
matrix[peaks[1].y][peaks[1].x] = 2;
matrix = passMap(matrix, dim, dim, 25, 2, 4)

-- Generate Snow on mountain peaks
matrix[peaks[0].y][peaks[0].x] = 3;
matrix[peaks[1].y][peaks[1].x] = 3;
matrix = passMap(matrix, dim, dim, 15, 3, 2)

-- Generate beaches
for y=0,dim-1 do
  for x=0,dim-1 do
    if matrix[y][x] == 1 or matrix[y][x] == 4 then
      if surround(matrix, x, y, dim, dim, 0) > 0 then
        matrix[y][x] = 5;
      end
    end
  end
end

for y=0,dim-1 do
  for x=0,dim-1 do
    if matrix[y][x] == 0 then
      if surround(matrix, x, y, dim, dim, 5) > 0 then
        matrix[y][x] = 6;
      end
    end
  end
end


-- C Hooks . . .
function retreiveMapValue(x, y)
  return matrix[y][x]
end

function getMapSize()
  return dim
end