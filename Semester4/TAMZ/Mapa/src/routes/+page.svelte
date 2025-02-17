<script lang="ts">
    import { degToRad } from '$lib';
    import { Feature } from 'ol';
    import Map from 'ol/Map';
    import View from 'ol/View';
    import { Point } from 'ol/geom';
    import TileLayer from 'ol/layer/Tile';
    import VectorLayer from 'ol/layer/Vector';
    import { fromLonLat } from 'ol/proj';
    import { OSM } from 'ol/source';
    import VectorSource from 'ol/source/Vector';
    import Icon from 'ol/style/Icon';
    import Style from 'ol/style/Style';
    import prettyMilliseconds from 'pretty-ms';
    import { onMount } from 'svelte';
    import { writable } from 'svelte/store';
    import IonButton from '../components/Ion/ionButton.svelte';
    import { currentLocation } from '../components/store.svelte';

    let map = writable<Map>();
    let icon: Feature;
    let blocked = false;
    let now = Date.now();
    let image: Icon;
    const maxToManualyGetPosition = 15 * 1000; //15s
    let center = true;
    let compassX = 0;
    let compassY = 0;
    let compassZ = 0;

    currentLocation.subscribe((value) => {
        if (!icon || !image) return;
        if (value.heading) {
            image.setRotation(degToRad(value.heading));
        }
        icon.setGeometry(new Point(fromLonLat([value.lon, value.lat])));

        if (center) {
            centerMap();
        }
    });

    onMount(() => {
        icon = new Feature(new Point([0, 0]));

        image = new Icon({
            src: '/heading.png',
            width: 25,
            height: 25,
            rotation: degToRad(0)
        });

        const style = new Style({
            image: image
        });

        icon.setStyle(style);

        navigator.geolocation.watchPosition(updatePosition, errorHandle, {
            enableHighAccuracy: true,
            maximumAge: 500
        });

        const interval = setInterval(() => {
            now = Date.now();

            if ($currentLocation) {
                if (now - $currentLocation.lastUpdate > maxToManualyGetPosition) {
                    navigator.geolocation.getCurrentPosition(updatePosition, errorHandle);
                }
            }
        }, 1000);

        window.addEventListener('deviceorientation', handleCompass);

        return () => {
            clearInterval(interval);
            window.removeEventListener('deviceorientation', handleCompass);
        };
    });

    const initializeMap = () => {
        map.set(
            new Map({
                target: 'map',
                layers: [
                    new TileLayer({
                        source: new OSM()
                    }),
                    new VectorLayer({
                        source: new VectorSource({ features: [icon] })
                    })
                ],
                view: new View({
                    center: fromLonLat([$currentLocation.lon, $currentLocation.lat]),
                    zoom: 16
                })
            })
        );
    };

    const updatePosition = (position: GeolocationPosition) => {
        blocked = false;

        currentLocation.set({
            lastUpdate: position.timestamp,
            lon: position.coords.longitude,
            lat: position.coords.latitude,
            precision: position.coords.accuracy,
            heading: position.coords.heading,
            speed: position.coords.speed,
            altitude: position.coords.altitude
                ? {
                      precision: position.coords.altitudeAccuracy as number,
                      value: position.coords.altitude
                  }
                : undefined
        });

        if ($map === undefined) {
            initializeMap();
        }
    };

    const errorHandle = (_: GeolocationPositionError) => {
        blocked = true;
    };

    const centerMap = () => {
        if (!$map) {
            return;
        }

        const currentView = $map.getView();
        const newView = new View({
            center: fromLonLat([$currentLocation.lon, $currentLocation.lat]),
            zoom: currentView.getZoom()
        });
        $map.setView(newView);
    };

    const toggleCenter = () => {
        center = !center;

        if (center) {
            centerMap();
        }
    };

    const printMs = (ms: number) => {
        if (ms < 0) {
            return 'Nyní';
        }

        return prettyMilliseconds(ms);
    };

    const handleCompass = (data: DeviceOrientationEvent) => {
        if (data.alpha == null || data.beta == null || data.gamma == null) {
            return;
        }

        compassX = data.alpha;
        compassY = data.beta;
        compassZ = data.gamma;
        //compass
    };
</script>

{#if blocked}
    <ion-text color="danger">
        <h1>Nepovolil jsi přístup k poloze.</h1>
    </ion-text>
{:else}
    <div style="width:100%;height:50%;" id="map"></div>
    <IonButton color={center ? 'success' : 'danger'} on:click={toggleCenter}>Centrování</IonButton>
    <ion-text>
        <h4>Pozice: (Poslední aktualizace: {printMs(now - ($currentLocation?.lastUpdate ?? 0))})</h4>
        Longitude: {$currentLocation?.lon}, Latitude: {$currentLocation?.lat}
        {$currentLocation?.heading}
        <br />
        Rychlost: {Math.round($currentLocation?.speed ?? 0 * 3.6)} km/h
    </ion-text>

    <h4>Orientace zařízení v prostoru:</h4>
    <i>(není relativní k reálným souřadnicím)</i>
    <div style="transform: rotateX({-compassY}deg) rotateY({compassZ}deg)" class="container">
        <img class="compass" src="/compass.png" alt="compass background" />
        <img style="z-index: {Math.abs(compassY) > 90 ? 10 : -5}" class="compass-bottom" src="/compass_bottom.png" alt="compass bottom" />
        <img style="transform: translateY(50%) rotateZ({-compassX - 45}deg);" class="arrow" src="/arrow.png" alt="compass arrow" />
    </div>
{/if}
